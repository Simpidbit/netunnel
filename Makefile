CC = # 随后配置
CCFLAGS = -std=c++14
INCLUDE_DIR = -I simwrap/simskt -I include

# OS == 0: Darwin
# OS == 1: Linux
# OS == 2: Windows
# OS == 3: Unknown
OS = 3
OS_NAME := $(shell uname -s)

ifeq ($(OS_NAME), Darwin)
	OS = 0
else ifeq ($(OS_NAME), Linux)
	OS = 1
else ifeq ($(OS_NAME), Windows_NT)
	OS = 2
else
	OS = 3
endif

# CC 配置
ifeq ($(OS), 0)
	# Darwin
	CC = clang++
else
	CC = g++
endif

# SIMSKT_LINK_LIB 配置
ifeq ($(OS), 2)
	# 在Windows下编译simskt需要链接: -lws2_32
	SIMSKT_LINK_LIB = -lws2_32
else
	SIMSKT_LINK_LIB = 
endif

# EFSW_LINK_LIB 配置
ifeq ($(OS), 0)	
	# Darwin
	EFSW_LINK_LIB = ./lib/darwin/libefsw_arm.a -framework CoreFoundation -framework CoreServices -lpthread
else ifeq ($(OS), 1)
	# Linux
	EFSW_LINK_LIB = ./lib/linux/libefsw_x86_64.a
else ifeq ($(OS), 2)
	# Windows
	EFSW_LINK_LIB = ./lib/windows/efsw_x86_64.lib
endif

ALL_LINK_LIB = $(SIMSKT_LINK_LIB) $(EFSW_LINK_LIB)

subm:
	git submodule init
	git submodule update

build_dir:
	mkdir -p build

simskt: subm build_dir
	$(CC) $(CCFLAGS) -c simwrap/simskt/simskt.cpp -o build/simskt.o

main: simskt
	$(CC) $(CCFLAGS) main.cpp build/simskt.o $(ALL_LINK_LIB) $(INCLUDE_DIR) -o main

clean:
	rm -rf build
	rm -f main
