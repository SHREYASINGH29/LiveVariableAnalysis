#!/bin/bash

set -x
LLVM_HOME=/usr/lib/llvm-12
CC=$LLVM_HOME/bin/clang
OPT=$LLVM_HOME/bin/opt

mkdir -p _build
pushd _build
cmake ..
make
popd
$CC -S -emit-llvm -o test.ll inter.c
if [ "$1" != "-i" ]; then
$OPT -instnamer  -load _build/*/*libLVA* -lva test.ll
else 
$OPT -instnamer -load _build/*/*IPLVA* -iplva test.ll
fi
#rm -rf _build test.bc
