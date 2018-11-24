# MAKEFILE

CC = gcc
CFLAGS = -Wextra -Werror -Wall -std=c99 -g -O3
LDLIBS = -lm

# EXEC = neural

SRC = neural_network.c main.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

# all :  ${EXEC}

all : main

-include ${DEP}

main : ${OBJ}

# xor : main_xor.o neural_network.o
# 	gcc main_xor.o neural_network.o -o xor ${CFLAGS} ${LDLIBS}
# -include ${DEP}
#
# neural : ${OBJ}
#
# main.o : main_xor.c
#	gcc -c main_xor.c ${CFLAGS} ${LDLIBS}
#
# neural_network.o : neural_network.c neural_network.h
#	gcc -c neural_network.c ${CFLAGS} ${LDLIBS}

clean : 
	rm -rf *.o

run : clean
	./${EXEC}

del : clean 
	rm -rf ${EXEC}

