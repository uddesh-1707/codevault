"""
Utility functions for CodeVault
"""

import re
from datetime import datetime
from typing import Optional
import logging

logger = logging.getLogger(__name__)


def sanitize_filename(name: str) -> str:
    """
    Sanitize a string to be used as a filename.
    Replaces invalid characters with underscores.
    """
    # Invalid characters for filesystems: / \ : * ? " < > |
    invalid_chars = r'[\/:*?"<>|]'
    sanitized = re.sub(invalid_chars, '_', name)
    
    # Remove leading/trailing spaces and dots
    sanitized = sanitized.strip(' .')
    
    # If empty, use a default
    if not sanitized:
        sanitized = "untitled"
    
    return sanitized


def get_extension(lang: str, extension_map: dict) -> str:
    """
    Get file extension for a given programming language.
    Returns default extension if language not found.
    """
    lang_lower = lang.lower()
    
    # Try exact match first
    if lang_lower in extension_map:
        return extension_map[lang_lower]
    
    # Try case-insensitive match
    for key, ext in extension_map.items():
        if key.lower() == lang_lower:
            return ext
    
    # Return default
    return extension_map.get('default', '.txt')


def format_date(timestamp: Optional[str]) -> str:
    """
    Convert timestamp to YYYY-MM-DD format.
    Handles both Unix timestamps and ISO strings.
    """
    if not timestamp:
        return datetime.utcnow().strftime("%Y-%m-%d")
    
    try:
        # Try parsing as ISO
        dt = datetime.fromisoformat(timestamp.replace('Z', '+00:00'))
        return dt.strftime("%Y-%m-%d")
    except:
        pass
    
    try:
        # Try parsing as Unix timestamp (seconds)
        timestamp_int = int(timestamp)
        dt = datetime.fromtimestamp(timestamp_int)
        return dt.strftime("%Y-%m-%d")
    except:
        pass
    
    # If all fails, return current date
    logger.warning(f"Could not parse timestamp: {timestamp}. Using current date.")
    return datetime.utcnow().strftime("%Y-%m-%d")


def parse_timestamp(timestamp: str) -> Optional[datetime]:
    """
    Parse a timestamp string to datetime object.
    Supports both Unix timestamps and ISO strings.
    """
    if not timestamp:
        return None
    
    try:
        # Try parsing as Unix timestamp
        timestamp_int = int(timestamp)
        return datetime.fromtimestamp(timestamp_int)
    except:
        pass
    
    try:
        # Try parsing as ISO string
        return datetime.fromisoformat(timestamp.replace('Z', '+00:00'))
    except:
        pass
    
    return None


def format_leetcode_submission(
    problem_name: str,
    code: str,
    timestamp: str,
    problem_link: str
) -> str:
    """
    Format a LeetCode submission as a file with header comments.
    """
    # Format date
    date_str = format_date(timestamp)
    
    # Create header
    header = f"""// Date: {date_str}
// Problem: {problem_name}
// Link: {problem_link}
// Code:

"""
    
    # Return with code
    return header + code