This directory should be passed to clang so it can resolve system includes (like
stddef.h) correctly. For example,

```bash
clang -resource-dir=/work/cquery/clang_resource_dir/
```

The path passed to clang does not include the "include" subdirectory.

This folder is copied from clang/LLVM. After running ./waf configure, this
should be the same as:

```bash
cp -r build/clang+llvm-4.0.0-x86_64-linux-gnu-ubuntu-14.04/lib/clang/4.0.0/include/
```
