#!/bin/bash
clear
echo "START OF TESTS"
./build.sh | grep "Assertion failed" -A3
echo "END OF TESTS"
