#!/bin/bash
# python3 -s ../../scripts/tirnsr_rx.py . && \
clear
basename="unittest_scan1_computes"
g++ -g -std=c++14 -I .  $basename.t-rx.out.cpp -Wunused-result -o unit-test-debug
./unit-test-debug
