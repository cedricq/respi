#!/bin/bash

echo $1

if [ $1 = TEST ]; then
	mkdir -p test
	cd test
	rm -rf *
    cmake -DTEST=ON ..
else
	mkdir -p build
	cd build
	rm -rf 
    cmake -DTEST=OFF -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ..
    #-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
fi