#
# This script formats all *.h and *.cc files in the "src/" directory.
#

$files=Get-ChildItem -Recurse src -Include *.h,*.cc
foreach ($file in $files) {
  Write-Host "Formatting" $file.FullName
  clang-format.exe -style=Chromium -i $file.FullName
}