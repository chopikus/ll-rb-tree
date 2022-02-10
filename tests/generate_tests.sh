#!/bin/bash
g++ gen_tests.cpp -O2 && ./a.out $1  > input.txt
