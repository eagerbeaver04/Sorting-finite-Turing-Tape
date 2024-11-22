#!/bin/sh

initial_dir=$(dirname "$(readlink -f "$0")")


if ! command -v cmake &> /dev/null; then
    echo "cmake is not installed. Please install cmake and try again."
    exit 1
fi

mkdir -p "$initial_dir/build"

cd "$initial_dir/build" || { echo "Failed to change to the build directory."; exit 1; }

cmake ..
if [ $? -ne 0 ]; then
    echo "Error while running cmake."
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "Error while running make."
    exit 1
fi

cd "$initial_dir" || { echo "Failed to exit build directory."; exit 1; }

mv "$initial_dir/build/tape_sorting" "$initial_dir/tape_sorting"
mv "$initial_dir/build/tests/run_tests" "$initial_dir/run_tests"

# rm -rf "$initial_dir/build"

mkdir -p "$initial_dir/tmp"

rm -rf "$initial_dir/tests/configs"
rm -rf "$initial_dir/tests/inputs"
mkdir "$initial_dir/tests/configs"
mkdir "$initial_dir/tests/inputs"

"$initial_dir/tape_sorting" "$initial_dir/config.txt"