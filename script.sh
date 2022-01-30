#!/bin/bash
g++ stupid.cpp -O2 -g && cat input.txt | ./a.out > output1.txt
echo "first done!"
g++ main.cpp -O2 -g && (cat input.txt | valgrind ./a.out > output2.txt)
diff output1.txt output2.txt
