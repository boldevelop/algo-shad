#!/bin/bash
set -e

path_to_folder=../contest$1/$2/
main=${path_to_folder}main.hpp

echo ${main}

clang-format -i ${main}
clang-tidy ${main} -- -std=c++17
