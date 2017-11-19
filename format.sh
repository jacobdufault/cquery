#!/bin/sh

#
# This script formats all *.h and *.cc files in the "src/" directory.
#
# Run it like this:
#
#   cd cquery/
#   ./format.sh
#

FILES=$(find src -name '*.cc' -o -name '*.h' | grep -v '/\.')
for FILE in $FILES; do
  echo "Formatting $FILE"
  clang-format -style=Chromium -i $FILE
done

