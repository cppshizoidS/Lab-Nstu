#!/bin/bash

if [ ! -d ../lab2_13/dynamic/build ]; then
	echo "u have to build lab2_13 (dynamic) before"
	exit
fi


if [ -d build ]; then
	rm -r build
fi

if [ ! -d lib ]; then
	mkdir lib
fi

if [ ! -f lib/my_lib.so ]; then
	cp ../lab2_13/dynamic/lib/my_lib.so lib/my_lib.so
fi

rm -f output-*

mkdir build

gcc client.c ../mytools.c -o build/client
gcc server.c ../mytools.c -o build/server

if [[ $1 = "--test" ]]; then
	./build/server
	./build/client file1.txt file2.txt
fi

