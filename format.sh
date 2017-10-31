#!/bin/sh

#
# This script formats all *.h and *.cc files in the "src/" directory.
#
# Run it like this:
#
#   cd cquery/
#   ./format.sh
#

FILES=$(find src -name '*.cc' -o -name '*.h')
clang-format -style=Chromium -verbose -i $FILES

