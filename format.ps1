#
# This script formats all *.h and *.cc files in the "src/" directory.
#

# FIXME: use clang-format from build/ directory.

$files=Get-ChildItem -Recurse src -Include *.h,*.cc |
    ? { $_.FullName -inotmatch '.vscode' }
foreach ($file in $files) {
  Write-Host "Formatting" $file.FullName
  .\build\LLVM-5.0.1-win64\bin\clang-format.exe -style=Chromium -i $file.FullName
}