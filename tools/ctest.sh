#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
CMAKE_SOURCE_DIR="$( dirname -- "${SCRIPT_DIR}" )"

if [ $# -eq 0 ]; then
    ARG1="${CMAKE_SOURCE_DIR}/build/Coverage"
else
    ARG1=$1
fi

CMAKE_BINARY_DIR="$( cd "$ARG1" && pwd )"

echo "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}"
echo "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}"

cd ${CMAKE_BINARY_DIR}
ctest
