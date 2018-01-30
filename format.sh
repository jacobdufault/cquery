#!/bin/sh

#
# This script formats all *.h and *.cc files in the "src/" directory.
#
# Run it like this:
#
#   cd cquery/
#   ./format.sh
#

# FIXME: use clang-format build build/ directory.

CLANG_FORMAT=$(find . -name clang-format)
echo "Using CLANG_FORMAT=$CLANG_FORMAT"

FILES=$(find src -name '*.cc' -o -name '*.h' | grep -v '/\.')
for FILE in $FILES; do
  echo "Formatting $FILE"
  $CLANG_FORMAT -style=Chromium -i $FILE
done

