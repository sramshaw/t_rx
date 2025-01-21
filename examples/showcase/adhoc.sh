#!/bin/bash
# python3 -s ../../scripts/tirnsr_rx.py . && \
clear
basename="unittest_publisher_pushes_several_sequences"
# g++ -g -std=c++14 -I .  $basename.t-rx.out.cpp -Wunused-result -o unit-test-debug  -g0 $basename.t-rx.out.hpp
# objcopy --strip-debug $basename.t-rx.out-hid.o
# g++ -g -c -std=c++14 -I .  $basename.t-rx.out-pub.hpp -Wunused-result -o $basename.t-rx.out-pub.o
# g++ $basename.t-rx.out-pub.o  $basename.t-rx.out-hid.o  -Wunused-result -o  $basename.t-rx.out.o 
g++ -g -std=c++14 -I .  $basename.t-rx.out.cpp -Wunused-result -o unit-test-debug
# ./unit-test-debug
