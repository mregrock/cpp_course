#!/bin/bash


mkdir -p cmake-build-debug
cd cmake-build-debug

cmake ..

cmake --build .
