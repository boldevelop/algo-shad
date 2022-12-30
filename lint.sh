#!/bin/bash
set -e

path_to_answer=../contest$1/$2/answer.cpp
echo ${path_to_answer}

clang-format -i ${path_to_answer}
clang-tidy ${path_to_answer} -- -std=c++17
