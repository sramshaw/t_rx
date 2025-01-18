#!/bin/bash
# echo '\n\n\n\n////////////////////  COMPILING pythagorian //////////////////' 
# rm showcase-debug
# g++ -g -std=c++14 -I . showcase.t-rx.out.cpp -o showcase-debug 
clear
for fullname in *.t-rx.out.cpp; do
    basename=$( echo $fullname | sed -e "s/\..*$//" )
    echo "\n\n////////////////////  COMPILING $basename //////////////////"
    echo 1, $fullname
    echo 2, $basename
    rm $basename.autorun 2>&1 
    g++ -g -std=c++14 -I .  $basename.t-rx.out.cpp -Wunused-result -o $basename.debug \
    && echo \
    && echo \
    && echo
done || exit 1
