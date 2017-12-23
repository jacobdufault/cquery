#!/bin/sh

#
# This script formats all *.h and *.cc files in the "src/" directory.
#
# Run it like this:
#
#   cd cquery/
#   ./format.sh
#

CLANG_FORMAT=./build/clang+llvm-4.0.0-x86_64-apple-darwin/bin/clang-format
echo "Using CLANG_FORMAT=$CLANG_FORMAT"

FILES=$(find src -name '*.cc' -o -name '*.h' | grep -v '/\.')
for FILE in $FILES; do
  echo "Formatting $FILE"
  $CLANG_FORMAT -style=Chromium -i $FILE
done

