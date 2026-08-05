#!/usr/bin/env python3
"""Setup script for CodeVault"""

from setuptools import setup, find_packages

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name="codevault-backup",
    version="1.0.0",
    author="CodeVault Contributors",
    author_email="your-email@example.com",
    description="Automated backup system for competitive programming solutions",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/your-username/codevault",
    packages=find_packages(),
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Build Tools",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
    ],
    python_requires=">=3.8",
    install_requires=[
        "requests>=2.31.0",
        "python-dotenv>=1.0.0",
        "PyYAML>=6.0.1",
        "GitPython>=3.1.40",
    ],
    entry_points={
        "console_scripts": [
            "codevault=src.main:main",
        ],
    },
)