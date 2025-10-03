CC = g++
CCFLAGS = -std=c++14
INCLUDE_DIR = -Isimwrap/simskt -Iinclude

# OS == 0: Darwin
# OS == 1: Linux
# OS == 2: Windows
# OS == 3: Unknown
OS = 3
OS_NAME := $(shell uname -s)

ifeq ($(OS_NAME), Darwin)
	OS = 0
	CC = clang++
else ifeq ($(OS_NAME), Linux)
	OS = 1
	CC = g++
else ifeq ($(OS_NAME), Windows_NT)
	OS = 2
else
	OS = 3
endif

# 在Windows下编译simskt需要链接: -lws2_32
ifeq ($(OS), 2)
	SIMSKT_LINK_LIB = -lws2_32
else
	SIMSKT_LINK_LIB = 
endif


subm:
	git submodule init
	git submodule update

build_dir:
	mkdir -p build

simskt: subm build_dir
	$(CC) $(CCFLAGS) -c simwrap/simskt/simskt.cpp $(SIMSKT_LINK_LIB) -o build/simskt.o

main: simskt
	$(CC) $(CCFLAGS) main.cpp build/simskt.o $(INCLUDE_DIR) -o main

clean:
	rm -rf build
	rm -f main

