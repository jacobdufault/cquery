#
# This script formats all *.h and *.cc files in the "src/" directory.
#

# FIXME: use clang-format from build/ directory.

$files=Get-ChildItem -Recurse src -Include *.h,*.cc |
    ? { $_.FullName -inotmatch '.vscode' }
foreach ($file in $files) {
  Write-Host "Formatting" $file.FullName
  clang-format.exe -style=Chromium -i $file.FullName
}