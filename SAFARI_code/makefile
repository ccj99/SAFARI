all:./bin/naive ./bin/safari
DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

NAIVE_SRC = $(wildcard ${DIR_SRC}/*.cpp) $(wildcard ${DIR_SRC}/naive/*.cpp)
SAFARI_SRC = $(wildcard ${DIR_SRC}/*.cpp) $(wildcard ${DIR_SRC}/safari/*.cpp)
NAIVE_OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${NAIVE_SRC})) 
SAFARI_OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SAFARI_SRC})) 

NAIVE_TARGET = naive
SAFARI_TARGET = safari

NAIVE_BIN_TARGET = ${DIR_BIN}/${NAIVE_TARGET}
SAFARI_BIN_TARGET = ${DIR_BIN}/${SAFARI_TARGET}

CC = g++
CFLAGS = -g -Wall -I${DIR_INC}

${NAIVE_BIN_TARGET}:${NAIVE_OBJ}
	$(CC) $(NAIVE_OBJ)  -o $@

${SAFARI_BIN_TARGET}:${SAFARI_OBJ}
	$(CC) $(SAFARI_OBJ)  -o $@

./obj/dataset.o:./src/dataset.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/rappor.o:./src/rappor.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/sampling_ldp.o:./src/sampling_ldp.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/grr.o:./src/grr.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/sh.o:./src/sh.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/util.o:./src/util.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/lasso.o:./src/lasso.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/olh.o:./src/olh.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/naive_main.o:./src/naive/naive_main.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/first_collect.o:./src/safari/first_collect.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/first_collect_with_lasso.o:./src/safari/first_collect_with_lasso.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/second_collect.o:./src/safari/second_collect.cpp
	$(CC) $(CFLAGS) -c  $< -o $@
	
./obj/safari_main.o:./src/safari/safari_main.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

./obj/learning_hierarchical.o:./src/safari/learning_hierarchical.cpp
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*
