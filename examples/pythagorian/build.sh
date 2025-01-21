#!/bin/bash
# from http://ptspts.blogspot.com/2013/12/how-to-make-smaller-c-and-c-binaries.html
# g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections showcase.t-rx.out.cpp -Wunused-result -o showcase.exe -Wl,--gc-sections 2>&1 | grep error 
# g++ -std=c++14 -I . -Os -fdata-sections -ffunction-sections -ftree-coalesce-vars -ftree-loop-im -m32 -fno-rtti -fno-stack-protector -fmerge-all-constants  -fno-math-errno -fno-exceptions -fvtable-gc showcase.t-rx.out.cpp -Wunused-result -o showcase.exe -Wl,--gc-sections 2>&1 | grep error 
# size showcase.exe && ./showcase.exe | c++filt -t
set -e
python3 -s ../../scripts/tirnsr_rx.py . && \
clear && \
for fullname in *.t-rx.out.cpp; do
    basename=$( echo $fullname | sed -e "s/\..*$//" )
    echo "////////////////////  COMPILING $basename //////////////////"
    echo 1, $fullname
    echo 2, $basename
    g++ -std=c++20 -I .   $basename.t-rx.out.cpp  -o $basename.autorun -pedantic -Os -fdata-sections -ffunction-sections -Wunused-result -Wl,--gc-sections #-ftree-coalesce-vars -ftree-loop-im 
    echo $?
    ./$basename.autorun
    rm $basename.autorun -f
    rm $basename.t-rx.out.cpp -f
    rm $basename.t-rx.out.hpp -f
    echo
    echo
    echo
done
