#!/bin/bash

FAST=solution
SLOW=sanitized_solution
TEST=test_solution
CXXFLAGS_RELEASE="-x c++ -std=c++14 -O2 -Wall -Werror"
CXXFLAGS_DEBUG="-fsanitize=address,undefined -x c++ -std=c++14 -O2 -Wall -Werror"
CXXFLAGS_TEST="-fsanitize=address,undefined -x c++ -std=c++14 -O2 -Wall"
export TMPDIR=`pwd`
export TEMP=`pwd`
export TMP=`pwd`
#mv $filename set.h

clang++ test.cpp $CXXFLAGS_RELEASE -o $FAST || exit 1
clang++ test.cpp $CXXFLAGS_DEBUG -o $SLOW || exit 1
clang++ test_debug.cpp $CXXFLAGS_TEST -o $TEST
