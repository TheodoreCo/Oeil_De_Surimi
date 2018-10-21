# MAKEFILE

CC = gcc
CFLAGS = -Wextra -Werror -Wall -std=c99 -O3
LDLIBS = -lm

EXEC = xor

SRC = main.c neural_network.c 
OBJ = ${SRC:.c=.o}

all : ${EXEC}

xor : main.o neural_network.o
	gcc main.o neural_network.o -o xor ${CFLAGS} ${LDLIBS}

main.o : main.c
	gcc -c main.c ${CFLAGS} ${LDLIBS}

neural_network.o : neural_network.c neural_network.h
	gcc -c neural_network.c ${CFLAGS} ${LDLIBS}

clean : 
	rm -rf *.o

run : clean
	./${EXEC}

del : clean 
	rm -rf ${EXEC}

