#!/bin/bash

# with clang90
# cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=debug -DCLANG_ROOT=$HOME/toolchains/clang90 -DSYSTEM_CLANG=ON  
# export LD_LIBRARY_PATH=$HOME/toolchains/clang90/lib:$LD_LIBRARY_PATH
# $HOME/SRC_jetson/cquery-kuba/build_clang90/debug/bin/cquery $@

$HOME/SRC_jetson/cquery-kuba/build/cquery $@

