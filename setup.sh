#!/bin/bash

if [ "$1" = "TEST" ]; then
	echo "$1"
	mkdir -p test
	cd test
	rm -rf *
    cmake -DTEST=ON ..
else
	mkdir -p build
	cd build
	rm -rf 
    cmake -DTEST=OFF -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DARM_TOOLCHAIN_DIR=/opt/gcc-arm-none-eabi-10.3-2021.10/bin ..
    #-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
fi