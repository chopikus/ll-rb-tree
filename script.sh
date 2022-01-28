#!/bin/bash
g++ stupid.cpp -O2 && cat input.txt | ./a.out > output1.txt
echo "first done!"
g++ tree_dirty.cpp -O2 && cat input.txt | ./a.out > output2.txt
#diff output1.txt output2.txt
