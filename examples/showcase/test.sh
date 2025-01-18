#!/bin/bash
clear
echo "START OF TESTS"
./build.sh 2>&1 | grep "Assertion failed" -A3 -B3
echo "END OF TESTS"
