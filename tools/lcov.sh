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

mkdir -p ${CMAKE_BINARY_DIR}/coverage
cd ${CMAKE_BINARY_DIR}/coverage

llvm-cov gcov -m -f -b `find ../test -name *.gcda` &> /dev/null
lcov --rc lcov_branch_coverage=1 --directory .. --base-directory .. --gcov-tool llvm-gcov.sh --capture -o coverage.info --include "${CMAKE_SOURCE_DIR}/src/*" --include "${CMAKE_SOURCE_DIR}/include/*"

mkdir -p ${CMAKE_SOURCE_DIR}/coverage
genhtml --rc lcov_branch_coverage=1 --demangle-cpp --branch-coverage --prefix=${CMAKE_SOURCE_DIR} coverage.info -o ${CMAKE_SOURCE_DIR}/coverage

