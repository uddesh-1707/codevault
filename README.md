# CodeVault

[![Automated Backup](https://img.shields.io/badge/Automated%20Backup-Enabled-brightgreen)](https://github.com/uddesh-1707/codevault/actions)
[![Python](https://img.shields.io/badge/Python-3.8%2B-blue)](https://python.org)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)

Automatically back up accepted LeetCode solutions to GitHub and keep them organized by date.

## Features

- Automated backups (default: every 6 hours via GitHub Actions)
- Solutions organized by date: `backup/YYYY-MM-DD/Problem_Name.ext`
- Smart deduplication: same-day duplicates are overwritten with the latest
- Email notifications for success, failure, and credential issues
- Queueing to protect against temporary GitHub downtime
- Support for many programming languages
- Secure credential handling via repository secrets

## Table of Contents

- [Quick Start](#quick-start)
- [Configuration](#configuration)
- [Backup Structure](#backup-structure)
- [Email Notifications](#email-notifications)
- [Troubleshooting](#troubleshooting)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Quick Start

Prerequisites

- Git
- Python 3.8+
- A LeetCode account with accepted solutions
- An email account for notifications (Gmail recommended)

Clone the repository (template branch recommended for user backups):

```bash
git clone --branch template https://github.com/uddesh-1707/codevault.git
cd codevault
```

For development or to inspect the main branch:

```bash
git clone https://github.com/uddesh-1707/codevault.git
cd codevault
git checkout main
```

Set up credentials

```bash
cp .env.template .env
# Edit .env and fill required values
```

Example `.env` entries

```env
# LeetCode
LEETCODE_SESSION=your_session_cookie_here
LEETCODE_CSRF_TOKEN=your_csrftoken_here

# GitHub
PAT_TOKEN=your_github_pat_token_here

# Email (Gmail app password recommended)
EMAIL_SENDER=your_email@gmail.com
EMAIL_PASSWORD=your_16_char_app_password
EMAIL_RECIPIENT=your_email@gmail.com
```

Install dependencies:

```bash
pip install -r requirements.txt
```

Configure `config.yaml` (set your LeetCode username):

```yaml
platforms:
  leetcode:
    enabled: true
    username: "your_leetcode_username"
```

Run locally:

```bash
python src/main.py
```

You should see logs indicating credentials validated, fetched submissions, and created backup files.

Deploy to GitHub Actions

1. Push your changes to your fork/branch
2. Add repository secrets under Settings → Secrets → Actions:

- `LEETCODE_SESSION`
- `LEETCODE_CSRF_TOKEN`
- `PAT_TOKEN`
- `EMAIL_SENDER`
- `EMAIL_PASSWORD`
- `EMAIL_RECIPIENT`

The workflow runs on the schedule defined in `.github/workflows/leetcode_backup.yml`.

## Configuration

Key settings live in `config.yaml`.

Example (partial):

```yaml
backup:
  root_dir: "backup"
  file_extension_map:
    cpp: ".cpp"
    python: ".py"

logging:
  level: "INFO"
  file: "backup.log"

git:
  commit_message: "Auto backup: {date}"
  max_retries: 3
```

## Backup Structure

Backups are stored under the `backup/` directory grouped by date:

```
backup/
├── 2026-08-01/
│   ├── Two_Sum.cpp
│   └── Reverse_List.cpp
└── 2026-08-02/
    └── Merge_Sorted_Array.cpp
```

Each file contains a small header with metadata (problem name, date, and link).

## Email Notifications

Notifications are sent for important events such as credential failures, successful backups, and push failures.

## Troubleshooting

- "LEETCODE_SESSION is missing or too short": refresh your LeetCode session cookie and update `.env`
- "Invalid CSRF token": refresh the CSRF token and update `.env`
- "SMTP Authentication failed": regenerate your Gmail app password and update `.env`
- "Git push failed": verify PAT permissions (repo + workflow)

Debug mode

```yaml
logging:
  level: "DEBUG"
```

Then inspect `backup.log`.

Reset local state

```bash
rm state.json
python src/main.py
```

## Project Structure

```
codevault/
├── .github/workflows/
│   └── leetcode_backup.yml
├── backup/
├── src/
│   ├── main.py
│   ├── state.py
│   ├── git_ops.py
│   └── fetchers/leetcode.py
├── .env.template
├── config.yaml
├── requirements.txt
├── README.md
└── LICENSE
```

## Contributing

Contributions welcome. Typical workflow:

```bash
git checkout -b feature/your-feature
# make changes
git commit -m "Add feature"
git push origin feature/your-feature
```

Open a pull request against the repository.

## Acknowledgments

- LeetCode
- GitHub Actions

