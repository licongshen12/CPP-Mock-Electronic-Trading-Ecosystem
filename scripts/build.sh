#!/bin/bash

CMAKE=$(which cmake)
NINJA=$(which ninja)

mkdir -p "$PROJECT_ROOT/cmake-build-release"
$CMAKE -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=$NINJA -G Ninja -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/cmake-build-release"

$CMAKE --build "$PROJECT_ROOT/cmake-build-release" --target clean -j 4
$CMAKE --build "$PROJECT_ROOT/cmake-build-release" --target all -j 4

mkdir -p "$PROJECT_ROOT/cmake-build-debug"
$CMAKE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=$NINJA -G Ninja -S "$PROJECT_ROOT" -B "$PROJECT_ROOT/cmake-build-debug"

$CMAKE --build "$PROJECT_ROOT/cmake-build-debug" --target clean -j 4
$CMAKE --build "$PROJECT_ROOT/cmake-build-debug" --target all -j 4
