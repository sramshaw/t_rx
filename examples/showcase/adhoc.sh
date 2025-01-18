#!/bin/bash
# python3 -s ../../scripts/tirnsr_rx.py . && \
clear
basename="unittest_scan_and_selectmany"
g++ -g -std=c++14 -I .  $basename.t-rx.out.cpp -Wunused-result -o $basename.debug
./$basename.debug
