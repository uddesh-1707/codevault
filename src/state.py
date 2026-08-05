"""
State Management for CodeVault
Tracks last successful fetch timestamp per platform
"""

import json
import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, Any, Optional

logger = logging.getLogger(__name__)


def get_default_state() -> Dict[str, Any]:
    """Return the default state structure"""
    return {
        "leetcode_last_ts": None,  # ISO format datetime (naive UTC)
        "last_run": None,
        "pending_backups": [],     # Dates with pending backups
        "queue_status": {          # Queue tracking
            "has_pending": False,
            "pending_count": 0,
            "last_queue_update": None
        }
    }


def load_state(state_file: str = "state.json") -> Dict[str, Any]:
    """
    Load state from JSON file.
    Returns default state if file doesn't exist.
    """
    state_path = Path(state_file)
    
    if not state_path.exists():
        logger.info("No state file found. Using default state.")
        return get_default_state()
    
    try:
        with open(state_path, 'r', encoding='utf-8') as f:
            state = json.load(f)
        logger.info(f"Loaded state from {state_file}")
        
        # Normalize timestamp: remove 'Z' if present (convert to naive UTC)
        if state.get("leetcode_last_ts"):
            ts = state["leetcode_last_ts"]
            if ts.endswith('Z'):
                state["leetcode_last_ts"] = ts.replace('Z', '')
        
        # Add new fields if missing
        if "pending_backups" not in state:
            state["pending_backups"] = []
        if "queue_status" not in state:
            state["queue_status"] = {
                "has_pending": False,
                "pending_count": 0,
                "last_queue_update": None
            }
        
        return state
    except Exception as e:
        logger.warning(f"Failed to load state: {e}. Using default.")
        return get_default_state()


def save_state(state: Dict[str, Any], state_file: str = "state.json") -> bool:
    """
    Save state to JSON file.
    Returns True on success, False on failure.
    """
    state_path = Path(state_file)
    
    try:
        # Update last run timestamp (naive UTC)
        state["last_run"] = datetime.utcnow().isoformat()
        
        # Ensure leetcode timestamp is naive (no 'Z')
        if state.get("leetcode_last_ts"):
            ts = state["leetcode_last_ts"]
            if ts.endswith('Z'):
                state["leetcode_last_ts"] = ts.replace('Z', '')
        
        with open(state_path, 'w', encoding='utf-8') as f:
            json.dump(state, f, indent=2)
        
        logger.debug(f"State saved to {state_file}")
        return True
    except Exception as e:
        logger.error(f"Failed to save state: {e}")
        return False


def update_leetcode_state(state: Dict[str, Any], timestamp: str) -> Dict[str, Any]:
    """
    Update LeetCode state with new timestamp.
    Expects timestamp in ISO format (naive UTC).
    """
    # Remove 'Z' if present (make it naive)
    if timestamp.endswith('Z'):
        timestamp = timestamp.replace('Z', '')
    state["leetcode_last_ts"] = timestamp
    return state


# ============================================================
# NEW: Queue Status Functions
# ============================================================

def update_queue_status(state: Dict[str, Any], has_pending: bool, count: int) -> bool:
    """
    Update queue status in state.
    """
    if "queue_status" not in state:
        state["queue_status"] = {
            "has_pending": False,
            "pending_count": 0,
            "last_queue_update": None
        }
    
    state["queue_status"]["has_pending"] = has_pending
    state["queue_status"]["pending_count"] = count
    state["queue_status"]["last_queue_update"] = datetime.utcnow().isoformat()
    
    if has_pending:
        logger.info(f"📌 Queue status: {count} pending items")
    else:
        logger.info("✅ Queue is empty")
    
    return save_state(state)


def mark_pending_backup(state: Dict[str, Any], date: str) -> bool:
    """
    Mark a date as having pending backup.
    """
    if "pending_backups" not in state:
        state["pending_backups"] = []
    
    if date not in state["pending_backups"]:
        state["pending_backups"].append(date)
        logger.info(f"📌 Marked {date} as pending backup")
    
    return save_state(state)


def clear_pending_backup(state: Dict[str, Any], date: str) -> bool:
    """
    Clear pending backup status for a date.
    """
    if "pending_backups" in state:
        state["pending_backups"] = [d for d in state["pending_backups"] if d != date]
        logger.info(f"✅ Cleared pending backup for {date}")
    
    return save_state(state)


def get_pending_backups(state: Dict[str, Any]) -> list:
    """
    Get list of dates with pending backups.
    """
    return state.get("pending_backups", [])