#!/bin/sh

# Copyright 2022(c) Ron Shabi

# This script invokes the Clang static analyzer (aka scan-build)

# check if not running in root dir

pwd=$(pwd)


# Check if script is running from 'scripts' directory.
# This is a light way of preventing misuse of the script.
# FIXME: Add more run-path preventions to this script
if [[ ${pwd} =~ "scripts" ]]; then
    echo "Please run me from the root directory"
    exit 1
fi

# Normal execution path
