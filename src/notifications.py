"""
Email Notification System for CodeVault
Sends alerts for backup status, failures, and credential issues
"""

import logging
import smtplib
import socket
import os
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from datetime import datetime
from typing import Dict, Optional, List
from pathlib import Path
from dotenv import load_dotenv

logger = logging.getLogger(__name__)


class EmailNotifier:
    """Email notification system using Gmail SMTP"""
    
    def __init__(self, config: Dict):
        """
        Initialize email notifier with configuration
        
        Args:
            config: Notification config from config.yaml
        """
        self.email_config = config.get('email', {})
        self.enabled = bool(config.get('enabled', False) and self.email_config.get('enabled', False))

        def resolve_value(value, default=None):
            if isinstance(value, str) and value.startswith('${') and value.endswith('}'):
                return os.getenv(value[2:-1], default)
            return value

        self.sender = resolve_value(self.email_config.get('sender'))
        
        # ============================================================
        # FIX: Read password directly from environment
        # ============================================================
        # Try multiple sources for password
        raw_password = resolve_value(self.email_config.get('password', ''), '')
        
        # If password is a template variable, resolve it
        if raw_password.startswith('${') and raw_password.endswith('}'):
            env_var = raw_password[2:-1]
            raw_password = os.getenv(env_var, '')
            logger.debug(f"Resolved {env_var} from environment")
        
        # If still empty, try loading .env directly
        if not raw_password:
            load_dotenv()
            raw_password = os.getenv('EMAIL_PASSWORD', '')
            logger.debug("Loaded EMAIL_PASSWORD from .env")
        
        # Preserve the secret value as-is apart from surrounding whitespace.
        self.password = raw_password.strip().strip('"\'')
        
        # Log the result (safe - only shows length)
        logger.info(f"📧 Email password length: {len(self.password)} characters")
        if len(self.password) == 16:
            logger.info("✅ Valid Gmail App Password (16 characters)")
        else:
            logger.warning(f"⚠️ Password length is {len(self.password)}, expected 16")
            logger.warning("   If this fails, regenerate your Gmail App Password")
            logger.warning("   Make sure there are NO spaces when copying")
        # ============================================================
        
        self.recipient = resolve_value(self.email_config.get('recipient'))
        self.smtp_server = resolve_value(self.email_config.get('smtp_server', 'smtp.gmail.com'), 'smtp.gmail.com') or 'smtp.gmail.com'
        smtp_port = resolve_value(self.email_config.get('smtp_port', 587), 587)
        try:
            self.smtp_port = int(smtp_port)
        except (TypeError, ValueError):
            self.smtp_port = 587
        
        # Validate configuration
        if self.enabled:
            if not all([self.sender, self.password, self.recipient]):
                logger.warning("📧 Email notifications enabled but missing credentials")
                self.enabled = False
            else:
                logger.info("📧 Email notifications configured successfully")
    
    def send_notification(
        self,
        subject: str,
        message: str,
        html_content: Optional[str] = None,
        is_critical: bool = False
    ) -> bool:
        """
        Send an email notification with improved connection handling
        """
        if not self.enabled:
            logger.debug("Email notifications disabled")
            return False
        
        try:
            # Create message
            msg = MIMEMultipart('alternative')
            msg['From'] = self.sender
            msg['To'] = self.recipient
            msg['Subject'] = f"[CodeVault] {subject}"
            
            # Add plain text version
            text_part = MIMEText(message, 'plain')
            msg.attach(text_part)
            
            # Add HTML version if provided
            if html_content:
                html_part = MIMEText(html_content, 'html')
                msg.attach(html_part)
            
            # Try multiple connection methods
            servers_to_try = [
                (self.smtp_server, self.smtp_port),  # Primary (hostname or IP)
                ('smtp.gmail.com', 587),              # Hostname fallback
            ]
            
            # Remove duplicates
            servers_to_try = list(dict.fromkeys(servers_to_try))
            
            for server_host, server_port in servers_to_try:
                try:
                    logger.debug(f"Attempting to connect to {server_host}:{server_port}...")
                    with smtplib.SMTP(server_host, server_port, timeout=10) as smtp_server:
                        smtp_server.starttls()
                        logger.debug("Logging in...")
                        smtp_server.login(self.sender, self.password)
                        logger.debug("Sending message...")
                        smtp_server.send_message(msg)
                    
                    logger.info(f"✅ Email notification sent via {server_host}:{server_port}")
                    return True
                    
                except (socket.gaierror, socket.timeout, smtplib.SMTPException) as e:
                    logger.debug(f"   Failed with {server_host}:{server_port} - {type(e).__name__}")
                    continue
                except Exception as e:
                    logger.debug(f"   Failed with {server_host}:{server_port} - {e}")
                    continue
            
            # If all attempts failed
            logger.error("❌ All connection attempts failed")
            return False
                
        except Exception as e:
            logger.error(f"❌ Failed to create email: {e}")
            return False
    
    def send_credential_failure(
        self,
        platform: str,
        credential_type: str,
        error: str
    ) -> bool:
        """Send credential failure notification"""
        
        subject = f"🚨 CREDENTIAL FAILURE - {platform}"
        
        message = f"""
🚨 CREDENTIAL FAILURE ALERT

Platform: {platform}
Credential: {credential_type}
Error: {error}
Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

ACTION REQUIRED IMMEDIATELY!

The {platform} {credential_type} has expired or is invalid.

Steps to fix:
1. Log in to {platform}
2. Generate/refresh your credentials
3. Update GitHub Secrets
4. Test the workflow manually

This backup system will continue to fail until credentials are updated.
        """
        
        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <style>
        body {{ font-family: Arial, sans-serif; color: #333; }}
        .critical {{ background-color: #dc3545; color: white; padding: 15px; border-radius: 5px; }}
        .info {{ background-color: #e7f3ff; padding: 10px; border-radius: 5px; }}
        .action {{ background-color: #fff3cd; padding: 15px; border-radius: 5px; border-left: 4px solid #ffc107; }}
        .steps {{ background-color: #f8f9fa; padding: 15px; border-radius: 5px; }}
        .footer {{ color: #666; font-size: 12px; margin-top: 20px; }}
    </style>
</head>
<body>
    <div class="critical">
        <h2>🚨 CREDENTIAL FAILURE ALERT</h2>
    </div>
    
    <div class="info">
        <p><strong>Platform:</strong> {platform}</p>
        <p><strong>Credential:</strong> {credential_type}</p>
        <p><strong>Error:</strong> {error}</p>
        <p><strong>Timestamp:</strong> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
    </div>
    
    <div class="action">
        <h3>⚠️ ACTION REQUIRED IMMEDIATELY!</h3>
        <p>The {platform} {credential_type} has expired or is invalid.</p>
    </div>
    
    <div class="steps">
        <h4>Steps to fix:</h4>
        <ol>
            <li>Log in to {platform}</li>
            <li>Generate/refresh your credentials</li>
            <li>Update GitHub Secrets</li>
            <li>Test the workflow manually</li>
        </ol>
    </div>
    
    <p><strong>This backup system will continue to fail until credentials are updated.</strong></p>
    
    <div class="footer">
        <p>CodeVault Automated Backup System</p>
    </div>
</body>
</html>
        """
        
        return self.send_notification(subject, message, html_content, is_critical=True)
    
    def send_backup_success(
        self,
        submissions_count: int,
        files_created: int,
        platform: str = "LeetCode"
    ) -> bool:
        """Send success notification"""
        
        subject = f"✅ Backup Successful - {platform}"
        
        message = f"""
CodeVault Backup Successful!

Platform: {platform}
Submissions found: {submissions_count}
Files created/updated: {files_created}
Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

All submissions have been backed up successfully.

View them at:
https://github.com/your-username/your-repo/tree/main/backup
        """
        
        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <style>
        body {{ font-family: Arial, sans-serif; color: #333; }}
        .success {{ background-color: #d4edda; padding: 15px; border-radius: 5px; border-left: 4px solid #28a745; }}
        .info {{ background-color: #e7f3ff; padding: 10px; border-radius: 5px; }}
        .footer {{ color: #666; font-size: 12px; margin-top: 20px; }}
    </style>
</head>
<body>
    <div class="success">
        <h2>✅ Backup Successful!</h2>
    </div>
    
    <div class="info">
        <p><strong>Platform:</strong> {platform}</p>
        <p><strong>Submissions found:</strong> {submissions_count}</p>
        <p><strong>Files created/updated:</strong> {files_created}</p>
        <p><strong>Timestamp:</strong> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
    </div>
    
    <p>All submissions have been backed up successfully.</p>
    
    <p><a href="https://github.com/your-username/your-repo/tree/main/backup">
        View backup files
    </a></p>
    
    <div class="footer">
        <p>CodeVault Automated Backup System</p>
    </div>
</body>
</html>
        """
        
        return self.send_notification(subject, message, html_content)
    
    def send_backup_failure(
        self,
        error: str,
        platform: str = "LeetCode",
        details: Optional[Dict] = None
    ) -> bool:
        """Send failure notification"""
        
        subject = f"❌ Backup Failed - {platform}"
        
        message = f"""
CodeVault Backup Failed!

Platform: {platform}
Error: {error}
Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

Action Required:
1. Check the GitHub Actions logs
2. Verify your credentials
3. Check if GitHub is operational

Details: {details if details else 'No additional details'}
        """
        
        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <style>
        body {{ font-family: Arial, sans-serif; color: #333; }}
        .error {{ background-color: #f8d7da; padding: 15px; border-radius: 5px; border-left: 4px solid #dc3545; }}
        .info {{ background-color: #e7f3ff; padding: 10px; border-radius: 5px; }}
        .action {{ background-color: #fff3cd; padding: 15px; border-radius: 5px; border-left: 4px solid #ffc107; }}
        .footer {{ color: #666; font-size: 12px; margin-top: 20px; }}
    </style>
</head>
<body>
    <div class="error">
        <h2>❌ Backup Failed!</h2>
    </div>
    
    <div class="info">
        <p><strong>Platform:</strong> {platform}</p>
        <p><strong>Error:</strong> {error}</p>
        <p><strong>Timestamp:</strong> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
    </div>
    
    <div class="action">
        <h3>Action Required:</h3>
        <ol>
            <li>Check the GitHub Actions logs</li>
            <li>Verify your credentials (LeetCode session, GitHub token)</li>
            <li>Check if GitHub is operational</li>
        </ol>
    </div>
    
    {f'<p><strong>Details:</strong> {details}</p>' if details else ''}
    
    <div class="footer">
        <p>CodeVault Automated Backup System</p>
    </div>
</body>
</html>
        """
        
        return self.send_notification(subject, message, html_content, is_critical=True)
    
    def send_github_downtime_alert(
        self,
        status: str,
        pending_count: int
    ) -> bool:
        """Send GitHub downtime notification"""
        
        subject = "⚠️ GitHub Service Degradation"
        
        message = f"""
⚠️ GitHub Service Degradation

Status: {status}
Pending submissions: {pending_count}
Timestamp: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}

GitHub is currently experiencing issues. Submissions have been queued.

Action:
- No action required
- System will retry on next scheduled run
- Submissions are safe and will be backed up when GitHub recovers
        """
        
        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <style>
        body {{ font-family: Arial, sans-serif; color: #333; }}
        .warning {{ background-color: #fff3cd; padding: 15px; border-radius: 5px; border-left: 4px solid #ffc107; }}
        .info {{ background-color: #e7f3ff; padding: 10px; border-radius: 5px; }}
        .footer {{ color: #666; font-size: 12px; margin-top: 20px; }}
    </style>
</head>
<body>
    <div class="warning">
        <h2>⚠️ GitHub Service Degradation</h2>
    </div>
    
    <div class="info">
        <p><strong>Status:</strong> {status}</p>
        <p><strong>Pending submissions:</strong> {pending_count}</p>
        <p><strong>Timestamp:</strong> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
    </div>
    
    <p>GitHub is currently experiencing issues. Submissions have been queued.</p>
    
    <h4>Action Required:</h4>
    <ul>
        <li>No action required</li>
        <li>System will retry on next scheduled run</li>
        <li>Submissions are safe and will be backed up when GitHub recovers</li>
    </ul>
    
    <div class="footer">
        <p>CodeVault Automated Backup System</p>
    </div>
</body>
</html>
        """
        
        return self.send_notification(subject, message, html_content)