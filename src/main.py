#!/usr/bin/env python3
"""
CodeVault Main Orchestrator - Phase 2: With Email Notifications
"""

import os
import sys
import logging
import copy
from pathlib import Path
from datetime import datetime, timedelta
from typing import Optional, Set, List

from dotenv import load_dotenv
import yaml

# Add src to path for imports
sys.path.insert(0, str(Path(__file__).parent))

from state import load_state, save_state, update_leetcode_state
from fetchers.leetcode import LeetCodeFetcher
from git_ops import git_commit_and_push, is_github_available
from notifications import EmailNotifier
from utils import (
    sanitize_filename,
    get_extension,
    format_date,
    format_leetcode_submission
)


def setup_logging(log_level="INFO", log_file=None):
    """Configure logging for the application"""
    log_format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
    log_datefmt = '%Y-%m-%d %H:%M:%S'
    
    # Set up root logger
    logger = logging.getLogger()
    logger.setLevel(getattr(logging, log_level.upper()))
    
    # Ensure UTF-8 encoding for console on Windows before the handler is bound.
    if sys.platform == 'win32':
        import io
        sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

    # Console handler with UTF-8 encoding support
    console = logging.StreamHandler(sys.stdout)
    console.setFormatter(logging.Formatter(log_format, log_datefmt))

    
    logger.addHandler(console)
    
    # File handler if specified (with UTF-8 encoding)
    if log_file:
        file_handler = logging.FileHandler(log_file, encoding='utf-8')
        file_handler.setFormatter(logging.Formatter(log_format, log_datefmt))
        logger.addHandler(file_handler)
    
    return logger


def load_config():
    """Load configuration from config.yaml"""
    config_path = Path("config.yaml")
    if config_path.exists():
        with open(config_path, 'r', encoding='utf-8') as f:
            return yaml.safe_load(f)
    return {}


def resolve_env_value(value, default=None):
    """Resolve ${VAR} templates from the environment."""
    if isinstance(value, str) and value.startswith('${') and value.endswith('}'):
        return os.getenv(value[2:-1], default)
    return value


def notification_allowed(notification_config: dict, event_name: str) -> bool:
    """Return True when a notification event is enabled by config."""
    send_on = set(notification_config.get('send_on', []))

    return event_name in send_on


def write_submission_file(
    submission: dict,
    backup_dir: str,
    extension_map: dict,
    seen_files: Set[str]
) -> Optional[Path]:
    """
    Write a submission to a file in the backup directory.
    Returns the Path of the created file, or None if failed.
    
    Behavior:
    - Same problem, same day: OVERWRITES with latest submission
    - Same problem, different day: Creates new file in new date folder
    - Different problem, same day: Creates new file
    """
    logger = logging.getLogger(__name__)
    
    try:
        # Parse timestamp
        timestamp = submission.get("timestamp", "")
        if not timestamp:
            logger.warning("Submission missing timestamp, skipping")
            return None
        
        # Format date for folder structure
        date_str = format_date(timestamp)
        
        # Sanitize problem name
        problem_name = submission.get("problem_name", "unknown")
        sanitized_name = sanitize_filename(problem_name)
        
        # Get file extension
        language = submission.get("language", "unknown")
        extension = get_extension(language, extension_map)
        
        # Build file path: backup/YYYY-MM-DD/problem_name.ext
        file_path = Path(backup_dir) / date_str / f"{sanitized_name}{extension}"
        
        # Create directories if they don't exist
        file_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Get problem link
        problem_link = submission.get("link", "")
        
        # Format the content with header
        content = format_leetcode_submission(
            problem_name=problem_name,
            code=submission.get("code", ""),
            timestamp=timestamp,
            problem_link=problem_link
        )
        
        # Check if file exists before writing
        file_existed = file_path.exists()
        
        # Write the file (OVERWRITE if exists)
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        # Mark as seen
        seen_files.add(str(file_path))
        
        # Log appropriate message
        if file_existed:
            logger.info(f"Updated file with latest submission: {file_path}")
        else:
            logger.info(f"Created new file: {file_path}")
        
        return file_path
        
    except Exception as e:
        logger.error(f"Failed to write submission file: {e}")
        return None


def get_latest_submission_timestamp(submissions: List[dict]) -> Optional[str]:
    """
    Get the timestamp of the most recent submission.
    Returns ISO format string or None if no submissions.
    """
    if not submissions:
        return None
    
    try:
        # Submissions are already sorted by timestamp (newest first)
        latest = submissions[0]
        timestamp = latest.get("timestamp", "")
        if timestamp:
            dt = datetime.fromtimestamp(int(timestamp))
            return dt.isoformat()
    except Exception as e:
        logging.getLogger(__name__).error(f"Failed to get latest timestamp: {e}")
    
    return None


def update_state_with_latest_submission(submissions: List[dict], state: dict, logger) -> bool:
    """
    Update state with the latest submission timestamp.
    Returns True if successful, False otherwise.
    """
    if not submissions:
        return False
    
    try:
        # Get the most recent submission timestamp
        latest_timestamp = get_latest_submission_timestamp(submissions)
        
        if latest_timestamp:
            state["leetcode_last_ts"] = latest_timestamp
            save_state(state)
            logger.info(f"State updated with timestamp: {latest_timestamp}")
            return True
        else:
            logger.warning("Could not update state: no timestamp available")
            return False
    except Exception as e:
        logger.error(f"Failed to update state: {e}")
        return False


def main():
    """Main orchestration logic with email notifications"""
    logger = logging.getLogger(__name__)
    
    # Load environment
    load_dotenv()
    
    # Load config
    config = load_config()
    
    # Setup logging
    log_config = config.get('logging', {})
    setup_logging(
        log_level=log_config.get('level', 'INFO'),
        log_file=log_config.get('file', 'backup.log')
    )

    logger.info(">>> CodeVault Phase 2: LeetCode Synchronizer starting...")
    
    # Initialize email notifier
    notification_config = config.get('notifications', {})
    
    # Resolve environment variables in email config
    if 'email' in notification_config:
        email_config = notification_config.get('email', {})
        
        # Resolve template variables from the environment.
        email_config['password'] = resolve_env_value(email_config.get('password', ''), '')
        email_config['sender'] = resolve_env_value(email_config.get('sender', ''), '')
        email_config['recipient'] = resolve_env_value(email_config.get('recipient', ''), '')
        email_config['smtp_server'] = resolve_env_value(email_config.get('smtp_server', 'smtp.gmail.com'), 'smtp.gmail.com')
        smtp_port = resolve_env_value(email_config.get('smtp_port', 587), 587)
        try:
            email_config['smtp_port'] = int(smtp_port)
        except (TypeError, ValueError):
            email_config['smtp_port'] = 587
        
        notification_config['email'] = email_config
    
    email_notifier = EmailNotifier(notification_config)
    if email_notifier.enabled:
        logger.info("📧 Email notifications enabled")
    
    # Verify LeetCode is enabled
    leetcode_config = config.get('platforms', {}).get('leetcode', {})
    if not leetcode_config.get('enabled', True):
        logger.info("LeetCode platform is disabled. Exiting.")
        return
    
    # Get credentials
    session_cookie = os.getenv('LEETCODE_SESSION')
    csrf_token = os.getenv('LEETCODE_CSRF_TOKEN')
    
    if not session_cookie or not csrf_token:
        error_msg = "Missing LeetCode credentials. Check .env file."
        logger.error(error_msg)
        logger.error("Required: LEETCODE_SESSION and LEETCODE_CSRF_TOKEN")
        
        # Send credential failure notification
        if email_notifier.enabled and notification_allowed(notification_config, 'credential_failure'):
            email_notifier.send_credential_failure(
                platform="LeetCode",
                credential_type="LEETCODE_SESSION/CSRF_TOKEN",
                error="Credentials not found in environment variables"
            )
        return
    
    username = leetcode_config.get('username')
    if not username:
        error_msg = "Missing LeetCode username in config.yaml"
        logger.error(error_msg)
        
        if email_notifier.enabled and notification_allowed(notification_config, 'credential_failure'):
            email_notifier.send_credential_failure(
                platform="LeetCode",
                credential_type="USERNAME",
                error="Username not found in config.yaml"
            )
        return
    
    logger.info(f"Fetching submissions for user: {username}")
    
    # Load state
    state = load_state()
    last_timestamp = state.get('leetcode_last_ts')
    
    # Use last_timestamp to fetch submissions since last backup
    if last_timestamp:
        logger.info(f"Last backup timestamp: {last_timestamp}")
        logger.info("Fetching submissions since last backup...")
    else:
        logger.info("No previous backup found. This is the first run.")
        logger.info("Fetching today's submissions only (to avoid backing up all history).")
        today_start = datetime.now().replace(hour=0, minute=0, second=0, microsecond=0)
        last_timestamp = today_start.isoformat()
        logger.info(f"First run: backing up submissions from {last_timestamp}")
    
    # Initialize fetcher
    fetcher = LeetCodeFetcher(username, session_cookie, csrf_token)
    
    try:
        # ============================================================
        # PROPER CREDENTIAL VALIDATION - REPLACES test_connection()
        # ============================================================
        logger.info("Validating LeetCode credentials...")
        is_valid, error_msg = fetcher.validate_credentials()
        
        if not is_valid:
            logger.error(f"❌ {error_msg}")
            
            # Send credential failure notification
            if email_notifier.enabled and notification_allowed(notification_config, 'credential_failure'):
                email_notifier.send_credential_failure(
                    platform="LeetCode",
                    credential_type="LEETCODE_SESSION",
                    error=error_msg
                )
            return
        
        # ============================================================
        # Fetch submissions - the fetcher filters by timestamp
        # ============================================================
        submissions = fetcher.fetch_submissions(last_timestamp, limit=50)

        if submissions is None:
            error_msg = "Failed to fetch submissions from LeetCode"
            logger.error(f"❌ {error_msg}")

            if email_notifier.enabled and notification_allowed(notification_config, 'backup_failure'):
                email_notifier.send_backup_failure(
                    error=error_msg,
                    details={
                        "platform": "LeetCode",
                        "stage": "fetch_submissions"
                    }
                )
            return
        
        # Scenario A: No new submissions found
        if not submissions:
            logger.info("No new submissions found.")
            # Update state with current time even when no new submissions
            current_time = datetime.now().isoformat()
            state["leetcode_last_ts"] = current_time
            save_state(state)
            logger.info(f"State updated with current time: {current_time}")
            
            return
        
        # Process ALL submissions since last_timestamp
        logger.info(f"Found {len(submissions)} new submission(s) since last backup")
        
        # Log submission dates for debugging (show first 5)
        for idx, sub in enumerate(submissions[:5]):
            try:
                sub_dt = datetime.fromtimestamp(int(sub.get("timestamp", 0)))
                logger.debug(f"  {idx+1}. {sub.get('problem_name')} ({sub_dt.strftime('%Y-%m-%d %H:%M:%S')})")
            except:
                pass
        
        if len(submissions) > 5:
            logger.debug(f"  ... and {len(submissions) - 5} more")
        
        # Get backup configuration
        backup_config = config.get('backup', {})
        root_dir = backup_config.get('root_dir', 'backup')
        extension_map = backup_config.get('file_extension_map', {})
        
        # Write files oldest-to-newest so the newest same-day submission wins.
        seen_files = set()
        files_created_or_updated = []
        write_failures = 0
        
        for sub in reversed(submissions):
            file_path = write_submission_file(sub, root_dir, extension_map, seen_files)
            if file_path:
                files_created_or_updated.append(file_path)
            else:
                write_failures += 1

        if write_failures:
            logger.error(f"❌ Failed to write {write_failures} submission file(s). Aborting backup.")
            if email_notifier.enabled and notification_allowed(notification_config, 'backup_failure'):
                email_notifier.send_backup_failure(
                    error="One or more submission files could not be written",
                    details={
                        "platform": "LeetCode",
                        "failed_writes": write_failures,
                        "attempted_files": len(submissions)
                    }
                )
            return
        
        if not files_created_or_updated:
            logger.warning("No files were created or updated.")
            # Still update state with the latest submission timestamp
            update_state_with_latest_submission(submissions, state, logger)
            return
        
        logger.info(f"Successfully created/updated {len(files_created_or_updated)} files")
        
        # Get Git configuration
        git_config = config.get('git', {})
        commit_message = git_config.get('commit_message', 'Auto backup: {date}')
        max_retries = git_config.get('max_retries', 3)
        retry_delay = git_config.get('retry_delay_seconds', 5)
        
        # Format commit message with current date
        commit_msg = commit_message.format(
            date=datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        )

        # Persist the new checkpoint before committing so state.json is included
        # in the same Git transaction as the backup files.
        previous_state = copy.deepcopy(state)
        if not update_state_with_latest_submission(submissions, state, logger):
            logger.error("❌ Failed to update state before Git commit.")
            return
        
        # Git transaction
        if git_commit_and_push(
            repo_path=".",
            message=commit_msg,
            max_retries=max_retries,
            retry_delay=retry_delay
        ):
            logger.info("✅ Backup completed successfully!")
        else:
            error_msg = "Git transaction failed. State not updated."
            logger.error(f"❌ {error_msg}")
            logger.warning("⚠️ Submissions are stored locally but not pushed to GitHub.")
            logger.warning("⚠️ Next run will re-process these submissions.")

            # Restore the previous checkpoint so a failed Git transaction does
            # not advance the state file.
            state.clear()
            state.update(previous_state)
            save_state(state)
            
            # Send failure email notification
            if email_notifier.enabled:
                github_available, github_status = is_github_available()
                if not github_available and notification_allowed(notification_config, 'github_downtime'):
                    email_notifier.send_github_downtime_alert(
                        status=github_status,
                        pending_count=len(files_created_or_updated)
                    )
                elif notification_allowed(notification_config, 'backup_failure'):
                    email_notifier.send_backup_failure(
                        error="Git push failed",
                        details={
                            "pending_files": len(files_created_or_updated),
                            "platform": "LeetCode",
                            "repository": "https://github.com/your-username/your-repo"
                        }
                    )
            
    except Exception as e:
        error_msg = str(e)
        logger.error(f"Error during backup: {e}")
        import traceback
        logger.debug(traceback.format_exc())
        
        # Send failure email notification with error details
        if email_notifier.enabled and notification_allowed(notification_config, 'backup_failure'):
            email_notifier.send_backup_failure(
                error=error_msg,
                details={
                    "traceback": traceback.format_exc()[:500],  # Limit to avoid huge emails
                    "platform": "LeetCode"
                }
            )
        return


if __name__ == "__main__":
    main()