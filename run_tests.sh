#!/bin/bash

# Run all Robot Framework tests in the 'suites' directory.

# Create log directory
#mkdir -p logs/robot

# Run the tests using robot command, and store result in log directory
robot -d $PWD/../logs/robot tests/robot/suites

# Check the return code of the 'robot' command.
# 0 means all tests passed.  Other values indicate failures.
if [ $? -eq 0 ]; then
  echo "All Robot Framework tests passed."
  exit 0
else
  echo "Robot Framework tests FAILED."
  exit 1
fi