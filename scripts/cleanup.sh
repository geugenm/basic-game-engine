#!/bin/bash

# Confirmation
echo "This script will delete the ../cache and ../build folders."
echo "Are you sure you want to continue? (y/n)"
read -r confirm

if [ "$confirm" != "y" ]; then
    exit 0
fi

# Deleting files
echo "Deleting files in ../cache and ../build folders..."
find ../cache ../build -type f -exec rm -f {} + 2>/dev/null || true

# Deleting directories
echo "Deleting ../cache and ../build folders..."
find ../cache ../build -type d -empty -delete 2>/dev/null || true
find ../cache ../build -type d -mindepth 1 -delete 2>/dev/null || true

# Status painting
echo "Deletion complete."