#!/usr/bin/env python

# This script tags the current commit with the current date and pushes it to
# GitHub. This will then trigger CI which will build the release and publish
# binaries.

import datetime
import subprocess
import sys

def RunOrExit(args):
  if subprocess.call(args) != 0:
    sys.exit(1)

if __name__ == "__main__":
  # Example: v2018-01-13@1820
  tag_name = datetime.datetime.now().strftime('v%F@%H%M')
  RunOrExit(['git', 'tag', tag_name])
  RunOrExit(['git', 'push', 'origin', tag_name])
