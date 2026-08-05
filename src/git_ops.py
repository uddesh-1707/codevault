"""
Git Operations with Atomic Transaction Logic and Queue Management
"""

import os
import json
import time
import logging
import subprocess
import requests
from pathlib import Path
from datetime import datetime, timedelta
from typing import List, Dict, Any, Optional, Tuple

logger = logging.getLogger(__name__)


def run_git_command(repo_path: str, args: list) -> tuple:
    """
    Execute a git command and return (success, output)
    """
    try:
        result = subprocess.run(
            ['git'] + args,
            cwd=repo_path,
            capture_output=True,
            text=True,
            encoding='utf-8',
            check=False
        )
        return result.returncode == 0, result.stdout + result.stderr
    except Exception as e:
        logger.error(f"Git command failed: {e}")
        return False, str(e)


def has_remote(repo_path: str = ".") -> bool:
    """
    Check if the repository has a remote configured.
    """
    success, output = run_git_command(repo_path, ['remote'])
    return success and output.strip() != ""


def has_commits(repo_path: str = ".") -> bool:
    """
    Check if the repository has any commits.
    """
    success, output = run_git_command(repo_path, ['log', '--oneline'])
    return success and output.strip() != ""


def get_current_branch(repo_path: str = ".") -> str:
    """
    Return the active branch name, falling back to main when detached.
    """
    success, output = run_git_command(repo_path, ['rev-parse', '--abbrev-ref', 'HEAD'])
    branch = output.strip() if success else ""
    if branch and branch != 'HEAD':
        return branch
    return 'main'


def rollback_last_commit(repo_path: str = ".") -> bool:
    """
    Roll back the most recent local commit safely.
    Uses git restore when there is no parent commit yet.
    """
    success, output = run_git_command(repo_path, ['rev-parse', 'HEAD~1'])
    if success:
        success, output = run_git_command(repo_path, ['reset', '--hard', 'HEAD~1'])
        if success:
            return True
        logger.error(f"Rollback via HEAD~1 failed: {output}")
        return False

    success, output = run_git_command(repo_path, ['restore', '--staged', '.'])
    if not success:
        logger.error(f"Rollback staging restore failed: {output}")
        return False

    success, output = run_git_command(repo_path, ['restore', '.'])
    if not success:
        logger.error(f"Rollback working tree restore failed: {output}")
        return False

    logger.info("✅ Rollback completed without parent commit")
    return True


# ============================================================
# NEW: Queue Management Functions
# ============================================================

def get_queue_file_path(repo_path: str = ".") -> Path:
    """Get the path to the queue file"""
    return Path(repo_path) / '.git' / 'pending_commits.json'


def is_github_available() -> Tuple[bool, str]:
    """
    Check if GitHub is reachable and operational.
    
    Returns:
        (is_available, status_message)
    """
    try:
        # Check GitHub status page
        response = requests.get(
            "https://www.githubstatus.com/api/v2/status.json",
            timeout=5
        )
        
        if response.status_code == 200:
            data = response.json()
            status = data.get('status', {})
            indicator = status.get('indicator', 'unknown')
            
            if indicator == 'none':
                return True, "All systems operational"
            else:
                return False, f"GitHub status: {status.get('description', 'Degraded')}"
        
        return False, f"GitHub status check failed (HTTP {response.status_code})"
        
    except Exception as e:
        return False, f"Cannot check GitHub status: {e}"


def get_pending_commits(repo_path: str = ".") -> List[Dict[str, Any]]:
    """Get all pending commits from the queue"""
    queue_file = get_queue_file_path(repo_path)
    
    if not queue_file.exists():
        return []
    
    try:
        with open(queue_file, 'r', encoding='utf-8') as f:
            pending = json.load(f)
        
        # Clean old entries (older than 7 days)
        cutoff = datetime.now() - timedelta(days=7)
        pending = [
            p for p in pending 
            if datetime.fromisoformat(p['timestamp']) > cutoff
        ]
        
        return pending
    except Exception as e:
        logger.error(f"Failed to read pending commits: {e}")
        return []


def save_to_queue(files: List[str], commit_message: str, repo_path: str = ".") -> bool:
    """
    Save pending changes to queue for later processing.
    
    Args:
        files: List of file paths to commit
        commit_message: Commit message
        repo_path: Repository path
        
    Returns:
        True if saved successfully, False otherwise
    """
    try:
        queue_file = get_queue_file_path(repo_path)
        pending = get_pending_commits(repo_path)
        
        # Add new entry
        pending.append({
            'timestamp': datetime.now().isoformat(),
            'files': files,
            'commit_message': commit_message,
            'date': datetime.now().strftime('%Y-%m-%d')
        })
        
        # Write to queue file
        queue_file.parent.mkdir(parents=True, exist_ok=True)
        with open(queue_file, 'w', encoding='utf-8') as f:
            json.dump(pending, f, indent=2)
        
        logger.info(f"📦 Queued {len(files)} files for later processing")
        logger.info(f"   Total pending: {len(pending)} entries")
        return True
        
    except Exception as e:
        logger.error(f"Failed to save to queue: {e}")
        return False


def clear_queue(repo_path: str = ".") -> bool:
    """Clear the queue after successful push"""
    try:
        queue_file = get_queue_file_path(repo_path)
        if queue_file.exists():
            queue_file.unlink()
            logger.info("✅ Queue cleared after successful push")
        return True
    except Exception as e:
        logger.error(f"Failed to clear queue: {e}")
        return False


def process_queued_commits(repo_path: str = ".") -> bool:
    """
    Process all queued commits.
    
    Returns:
        True if all processed successfully, False otherwise
    """
    pending = get_pending_commits(repo_path)
    if not pending:
        return True

    branch_name = get_current_branch(repo_path)
    
    logger.info(f"📦 Processing {len(pending)} queued entries...")
    
    # Check if GitHub is available
    is_available, status = is_github_available()
    if not is_available:
        logger.warning(f"⚠️ GitHub unavailable: {status}")
        logger.info("   Keeping queue for later")
        return False
    
    for idx, entry in enumerate(pending):
        try:
            files = entry.get('files', [])
            commit_msg = entry.get('commit_message', 'Auto backup (delayed)')
            
            logger.info(f"   Processing queued entry {idx+1}/{len(pending)}")
            
            # Add files
            for file in files:
                success, output = run_git_command(repo_path, ['add', file])
                if not success:
                    logger.error(f"   ❌ Failed to add file: {file}")
                    return False
            
            # Commit
            success, output = run_git_command(repo_path, ['commit', '-m', commit_msg])
            if not success:
                if "nothing to commit" in output.lower():
                    logger.info(f"   ℹ️ Nothing to commit for entry {idx+1}")
                else:
                    logger.error(f"   ❌ Failed to commit: {output}")
                    return False
            
            logger.info(f"   ✅ Successfully committed queued entry")
            
        except Exception as e:
            logger.error(f"   ❌ Failed to process queued entry: {e}")
            return False
    
    # Push all queued commits
    logger.info("📤 Pushing all queued commits...")
    success, output = run_git_command(repo_path, ['push', 'origin', branch_name])
    if success:
        clear_queue(repo_path)
        logger.info("✅ All queued commits processed and pushed successfully")
        return True
    else:
        logger.error(f"❌ Failed to push queued commits: {output}")
        return False


# ============================================================
# MAIN FUNCTION - Updated with Queue Management
# ============================================================

def git_commit_and_push(
    repo_path: str = ".",
    message: str = None,
    max_retries: int = 3,
    retry_delay: int = 5
) -> bool:
    """
    Atomic Git transaction: add, commit, pull, push with rollback.
    Includes queue management for GitHub downtime.
    
    Returns True if successful, False if failed (with rollback).
    """
    logger.info("Starting Git transaction with queue management...")
    
    # Check if we're in a git repo
    success, _ = run_git_command(repo_path, ['rev-parse', '--git-dir'])
    if not success:
        logger.error("Not a git repository. Please run 'git init' first.")
        return False
    
    # ============================================================
    # STEP 1: Process any queued commits first
    # ============================================================
    pending = get_pending_commits(repo_path)
    if pending:
        logger.info(f"📦 Found {len(pending)} pending commits in queue...")
        
        is_available, status = is_github_available()
        if is_available:
            logger.info("✅ GitHub is available. Processing queue...")
            if process_queued_commits(repo_path):
                logger.info("✅ All queued commits processed")
            else:
                logger.warning("⚠️ Failed to process some queued commits")
                # Continue anyway - will retry next time
        else:
            logger.warning(f"⚠️ GitHub unavailable: {status}")
            logger.info("   Keeping queue for later")
            # Don't return False - still save current changes
    
    # ============================================================
    # STEP 2: Commit current changes
    # ============================================================
    # Default message if not provided
    if not message:
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        message = f"Auto backup: {timestamp}"
    
    # Step 1: Add all changes
    success, output = run_git_command(repo_path, ['add', '-A'])
    if not success:
        logger.error(f"Failed to add files: {output}")
        return False
    
    # Step 2: Commit
    success, output = run_git_command(
        repo_path, ['commit', '-m', message]
    )
    
    if not success:
        if "nothing to commit" in output.lower():
            logger.info("No changes to commit.")
            # No changes to commit - still return True if queue is empty
            return not get_pending_commits(repo_path)
        logger.error(f"Failed to commit: {output}")
        return False
    
    logger.info(f"Committed: {message}")
    
    # Step 3: Get list of committed files for queue (if needed)
    # Get the list of files in the commit
    committed_files = []
    success, output = run_git_command(
        repo_path, ['diff', '--name-only', 'HEAD~1..HEAD']
    )
    if success and output.strip():
        committed_files = output.strip().split('\n')
    
    # Check if remote exists
    remote_exists = has_remote(repo_path)
    
    if not remote_exists:
        logger.info("No remote configured. Commit saved locally only.")
        logger.info("To push to GitHub, run: git remote add origin <your-repo-url>")
        return True
    
    # ============================================================
    # STEP 3: Push with retry and queue fallback
    # ============================================================
    branch_name = get_current_branch(repo_path)

    # Check if GitHub is available before attempting push
    is_available, status = is_github_available()
    if not is_available:
        logger.warning(f"⚠️ GitHub unavailable: {status}")
        logger.info("📦 Saving commit to queue for later...")
        
        if committed_files and save_to_queue(committed_files, message, repo_path):
            logger.info("✅ Changes saved to queue. Will retry on next run.")
            # Rollback the local commit to avoid confusion
            if rollback_last_commit(repo_path):
                logger.info("Local commit rolled back (saved in queue).")
            else:
                logger.error("Rollback failed! Manual intervention required.")
            return False
        else:
            logger.error("Failed to save to queue!")
            return False
    
    # Push with retry
    for attempt in range(1, max_retries + 1):
        logger.info(f"Push attempt {attempt}/{max_retries}")
        
        # Try to pull with rebase (only if remote exists and has commits)
        if has_commits(repo_path):
            success, output = run_git_command(
                repo_path, ['pull', '--rebase', 'origin', branch_name]
            )
            if not success:
                if "couldn't find remote ref" in output.lower():
                    logger.info("Remote branch doesn't exist yet. Will push directly.")
                else:
                    logger.warning(f"Pull failed (attempt {attempt}): {output}")
                    if attempt < max_retries:
                        time.sleep(retry_delay)
                        continue
                    else:
                        logger.error("All pull attempts failed. Saving to queue...")
                        if committed_files and save_to_queue(committed_files, message, repo_path):
                            # Rollback local commit
                            rollback_last_commit(repo_path)
                            logger.info("Changes saved to queue. Will retry next run.")
                            return False
                        else:
                            # Rollback
                            rollback_last_commit(repo_path)
                            return False
        
        # Push
        success, output = run_git_command(
            repo_path, ['push', '-u', 'origin', branch_name]
        )
        if success:
            logger.info("Git push successful!")
            # Clear queue if any (should be empty already)
            clear_queue(repo_path)
            return True
        else:
            # If push fails because remote doesn't exist
            if "no such remote" in output.lower() or "remote origin not found" in output.lower():
                logger.error("Remote 'origin' not found. Please add remote first.")
                logger.info("Run: git remote add origin <your-github-repo-url>")
                return True  # Commit is already saved locally
            
            # Check if this is a GitHub availability issue
            is_available, status = is_github_available()
            if not is_available:
                logger.warning(f"⚠️ GitHub unavailable during push: {status}")
                logger.info("📦 Saving commit to queue for later...")
                if committed_files and save_to_queue(committed_files, message, repo_path):
                    # Rollback local commit
                    rollback_last_commit(repo_path)
                    logger.info("Changes saved to queue. Will retry next run.")
                    return False
                else:
                    # Rollback
                    rollback_last_commit(repo_path)
                    return False
            
            logger.warning(f"Push failed (attempt {attempt}): {output}")
            if attempt < max_retries:
                time.sleep(retry_delay)
            else:
                logger.error("All push attempts failed. Saving to queue...")
                if committed_files and save_to_queue(committed_files, message, repo_path):
                    # Rollback local commit
                    rollback_last_commit(repo_path)
                    logger.info("Changes saved to queue. Will retry next run.")
                    return False
                else:
                    # Rollback
                    rollback_last_commit(repo_path)
                    logger.error("Failed to save to queue! Rollback complete.")
                    return False
    
    return False