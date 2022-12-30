#!/bin/bash
set -e

path_to_folder=../contest$1/$2/
answer=${path_to_folder}answer.cpp
main=${path_to_folder}main.hpp

echo ${answer}
echo ${main}

clang-format -i ${answer} ${main}
clang-tidy ${answer} ${main} -- -std=c++17
