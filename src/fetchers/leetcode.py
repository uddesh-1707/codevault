"""
LeetCode Fetcher - Complete GraphQL Implementation
"""

import json
import logging
import requests
from typing import List, Dict, Any, Optional, Tuple
from datetime import datetime

logger = logging.getLogger(__name__)


class LeetCodeFetcher:
    """LeetCode submission fetcher using GraphQL API"""
    
    def __init__(self, username: str, session_cookie: str, csrf_token: str):
        """Initialize the LeetCode fetcher with credentials"""
        self.username = username
        self.session_cookie = session_cookie
        self.csrf_token = csrf_token
        self.api_url = "https://leetcode.com/graphql"
        self.headers = {
            "Content-Type": "application/json",
            "Cookie": f"LEETCODE_SESSION={session_cookie}; csrftoken={csrf_token}",
            "x-csrftoken": csrf_token,
            "Referer": "https://leetcode.com",
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36"
        }
        
    def fetch_recent_submissions(self, limit: int = 50) -> Optional[List[Dict[str, Any]]]:
        """
        Fetch recent submissions for the user using GraphQL.
        
        Args:
            limit: Number of recent submissions to fetch (max 50)
            
        Returns:
            List of submission dicts with fields:
            - id: submission ID
            - title: problem title
            - titleSlug: URL slug for the problem
            - timestamp: Unix timestamp
            - statusDisplay: "Accepted", "Wrong Answer", etc.
            - lang: programming language
        """
        query = """
        query recentSubmissions($username: String!, $limit: Int!) {
            recentSubmissionList(username: $username, limit: $limit) {
                id
                title
                titleSlug
                timestamp
                statusDisplay
                lang
            }
        }
        """
        
        variables = {
            "username": self.username,
            "limit": limit
        }
        
        payload = {
            "query": query,
            "variables": variables
        }
        
        try:
            response = requests.post(
                self.api_url,
                headers=self.headers,
                json=payload,
                timeout=30
            )
            
            if response.status_code != 200:
                logger.error(f"API request failed with status: {response.status_code}")
                logger.debug(f"Response: {response.text}")
                return None
            
            data = response.json()
            
            # Check for GraphQL errors
            if "errors" in data:
                logger.error(f"GraphQL errors: {data['errors']}")
                return None
            
            submissions = data.get("data", {}).get("recentSubmissionList", [])
            logger.info(f"Fetched {len(submissions)} recent submissions")
            return submissions
            
        except requests.exceptions.RequestException as e:
            logger.error(f"Request failed: {e}")
            return None
        except json.JSONDecodeError as e:
            logger.error(f"Failed to parse response: {e}")
            return None
    
    def fetch_submission_code(self, submission_id: int) -> Optional[str]:
        """
        Fetch the code for a specific submission using GraphQL.
        
        Args:
            submission_id: The submission ID to fetch code for
            
        Returns:
            The submission code as string, or None if failed
        """
        query = """
        query submissionDetails($submissionId: Int!) {
            submissionDetails(submissionId: $submissionId) {
                code
            }
        }
        """
        
        variables = {
            "submissionId": submission_id
        }
        
        payload = {
            "query": query,
            "variables": variables
        }
        
        try:
            response = requests.post(
                self.api_url,
                headers=self.headers,
                json=payload,
                timeout=30
            )
            
            if response.status_code != 200:
                logger.error(f"Code fetch failed with status: {response.status_code}")
                return None
            
            data = response.json()
            
            if "errors" in data:
                logger.error(f"GraphQL errors fetching code: {data['errors']}")
                return None
            
            code = data.get("data", {}).get("submissionDetails", {}).get("code")
            
            if code is None:
                logger.warning(f"No code found for submission ID: {submission_id}")
                return None
            
            return code
            
        except requests.exceptions.RequestException as e:
            logger.error(f"Request failed for submission {submission_id}: {e}")
            return None
        except json.JSONDecodeError as e:
            logger.error(f"Failed to parse response for submission {submission_id}: {e}")
            return None
    
    def filter_accepted_submissions(
        self, 
        submissions: List[Dict[str, Any]], 
        last_timestamp: Optional[str] = None
    ) -> Optional[List[Dict[str, Any]]]:
        """
        Filter submissions to only include accepted ones after last_timestamp.
        
        Args:
            submissions: List of submission dicts from fetch_recent_submissions
            last_timestamp: ISO timestamp string or None for all
            
        Returns:
            Filtered list of submissions
        """
        accepted = []
        
        # Convert last_timestamp to datetime for comparison
        last_dt = None
        if last_timestamp:
            try:
                # Handle both 'Z' and '+00:00' formats
                ts = last_timestamp.replace('Z', '+00:00')
                last_dt = datetime.fromisoformat(ts)
                # Make it naive for comparison with naive timestamps
                last_dt = last_dt.replace(tzinfo=None)
                logger.debug(f"Filtering submissions after: {last_dt}")
            except Exception as e:
                logger.warning(f"Could not parse last_timestamp '{last_timestamp}': {e}")
                last_dt = None
        
        for sub in submissions:
            # Only include accepted submissions
            if sub.get("statusDisplay") != "Accepted":
                continue
            
            # Check if submission is newer than last_timestamp
            if last_dt:
                try:
                    # Convert Unix timestamp to datetime (naive)
                    sub_dt = datetime.fromtimestamp(int(sub["timestamp"]))
                    
                    # Only include submissions AFTER the last backup
                    # Note: Using > (greater than) to avoid duplicates
                    if sub_dt <= last_dt:
                        logger.debug(f"Skipping submission from {sub_dt} (<= {last_dt})")
                        continue
                except Exception as e:
                    logger.warning(f"Could not parse timestamp: {sub.get('timestamp')}: {e}")
                    continue
            
            accepted.append(sub)
        
        logger.info(f"Found {len(accepted)} accepted submissions (filtered from {len(submissions)})")
        return accepted
    
    def get_problem_link(self, title_slug: str) -> str:
        """
        Generate the LeetCode problem URL.
        """
        return f"https://leetcode.com/problems/{title_slug}/"
    
    def fetch_submissions(
        self, 
        last_timestamp: Optional[str] = None,
        limit: int = 50
    ) -> List[Dict[str, Any]]:
        """
        Main method: Fetch all accepted submissions newer than last_timestamp.
        
        Args:
            last_timestamp: ISO timestamp to filter submissions
            limit: Number of recent submissions to fetch (default 50)
            
        Returns:
            List of submission dicts with keys:
            - problem_name: Full problem name
            - title_slug: URL slug
            - link: Full LeetCode URL
            - code: The submission code
            - language: Programming language
            - timestamp: Unix timestamp string
            - submission_id: The submission ID
        """
        logger.info(f"Fetching submissions for {self.username}...")
        logger.info(f"Limit: {limit}, Filter: since {last_timestamp or 'beginning'}")
        
        # Step 1: Get recent submissions
        submissions = self.fetch_recent_submissions(limit)
        if submissions is None:
            logger.error("Failed to fetch recent submissions")
            return None

        if not submissions:
            logger.warning("No submissions fetched")
            return []
        
        # Step 2: Filter accepted and new submissions
        accepted = self.filter_accepted_submissions(submissions, last_timestamp)
        if not accepted:
            logger.info("No new accepted submissions found")
            return []
        
        # Step 3: Fetch code for each accepted submission
        results = []
        for idx, sub in enumerate(accepted):
            submission_id = sub.get("id")
            if not submission_id:
                logger.warning("Submission missing ID, skipping")
                continue
            
            logger.debug(f"Fetching code for submission {idx+1}/{len(accepted)}: {sub.get('title')}")
            code = self.fetch_submission_code(int(submission_id))
            if code is None:
                logger.warning(f"Failed to fetch code for submission {submission_id}, skipping")
                continue
            
            # Build result dict
            result = {
                "problem_name": sub.get("title", "Unknown"),
                "title_slug": sub.get("titleSlug", ""),
                "link": self.get_problem_link(sub.get("titleSlug", "")),
                "code": code,
                "language": sub.get("lang", "unknown"),
                "timestamp": sub.get("timestamp", ""),
                "submission_id": submission_id
            }
            results.append(result)
            
            logger.debug(f"  ✓ Fetched: {result['problem_name']} ({result['language']})")
        
        logger.info(f"✅ Successfully fetched {len(results)} submissions with code")
        return results
    
    def test_connection(self) -> bool:
        """
        Test if the fetcher can connect to LeetCode API.
        Returns True if successful, False otherwise.
        """
        try:
            logger.info("Testing connection to LeetCode API...")
            submissions = self.fetch_recent_submissions(1)
            success = len(submissions) > 0
            if success:
                logger.info("✅ Connection successful!")
            else:
                logger.error("❌ Connection failed - no submissions returned")
            return success
        except Exception as e:
            logger.error(f"❌ Connection test failed: {e}")
            return False

    def validate_credentials(self) -> Tuple[bool, str]:
        """
        Properly validate LeetCode credentials including CSRF token.
        Returns (is_valid, error_message)
        """
        try:
            # First, check if session cookie is present
            if not self.session_cookie or len(self.session_cookie) < 10:
                logger.error("❌ LEETCODE_SESSION is missing or too short")
                return False, "LEETCODE_SESSION is missing or too short"
            
            # Check CSRF token
            if not self.csrf_token or len(self.csrf_token) < 10:
                logger.error("❌ CSRF token is missing or too short")
                return False, "CSRF token is missing or too short"
            
            logger.info(f"🔍 Validating credentials for user: {self.username}")
            logger.debug(f"📧 Session cookie length: {len(self.session_cookie)}")
            logger.debug(f"🔑 CSRF token length: {len(self.csrf_token)}")
            
            # Try to fetch user profile - this validates both session and CSRF
            query = """
            query getCurrentUser {
                user {
                    username
                    lastName
                    firstName
                }
            }
            """
            
            payload = {"query": query, "variables": {}}
            
            # First attempt with both cookies
            response = requests.post(
                self.api_url,
                headers=self.headers,
                json=payload,
                timeout=30
            )
            
            logger.debug(f"📡 Response status: {response.status_code}")
            
            if response.status_code == 200:
                data = response.json()
                
                # Check for GraphQL errors
                if "errors" in data:
                    error_messages = [str(e) for e in data["errors"]]
                    error_str = " ".join(error_messages).lower()
                    
                    # Check if CSRF is the issue
                    if "csrf" in error_str or "token" in error_str:
                        logger.error(f"❌ CSRF token validation failed")
                        return False, "Invalid CSRF token - please refresh your csrftoken"
                    
                    # Check if session is the issue
                    if "session" in error_str or "authentication" in error_str or "invalid" in error_str:
                        logger.error(f"❌ Session validation failed")
                        return False, "Invalid LEETCODE_SESSION - please refresh your session"
                    
                    logger.error(f"❌ GraphQL error: {data['errors']}")
                    return False, f"GraphQL error: {data['errors']}"
                
                # Check if user data exists
                user_data = data.get("data", {}).get("user")
                if not user_data:
                    logger.error(f"❌ User data not found")
                    return False, "User data not found"
                
                user_name = user_data.get("username", self.username)
                first_name = user_data.get("firstName", "")
                last_name = user_data.get("lastName", "")
                full_name = f"{first_name} {last_name}".strip() or user_name
                
                logger.info(f"✅ Credentials validated successfully!")
                logger.info(f"   User: {full_name}")
                logger.info(f"   Session: OK")
                logger.info(f"   CSRF Token: OK")
                return True, "Valid credentials"
            
            elif response.status_code == 403:
                # 403 often means CSRF token is invalid
                logger.error("❌ HTTP 403 - CSRF token likely invalid")
                return False, "Invalid CSRF token (HTTP 403) - please refresh your csrftoken"
            
            elif response.status_code == 401:
                # 401 means session is invalid
                logger.error("❌ HTTP 401 - Session invalid")
                return False, "Invalid LEETCODE_SESSION (HTTP 401) - please refresh your session"
            
            else:
                logger.error(f"❌ API returned status {response.status_code}")
                logger.error(f"📄 Response preview: {response.text[:200]}...")
                return False, f"API returned status {response.status_code}"
                
        except requests.exceptions.Timeout:
            logger.error("❌ Connection timeout - check your internet")
            return False, "Connection timeout - check your internet"
        except requests.exceptions.ConnectionError:
            logger.error("❌ Connection error - cannot reach LeetCode")
            return False, "Connection error - cannot reach LeetCode"
        except Exception as e:
            logger.error(f"❌ Validation error: {e}")
            import traceback
            logger.error(traceback.format_exc())
            return False, f"Validation error: {e}"