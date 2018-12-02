# MAKEFILE
#-Werror

CC = gcc
CFLAGS = -Wextra -Wall -std=c99 -g -O2
LDLIBS = -lm

EXEC = main

SRC = neural_network.c main.c  
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all : main

-include ${DEP}

main : ${OBJ}

clean : 
	rm -rf *.o 
	rm -rf *.d
	rm ${EXEC} 

del : clean 
	rm -rf xor.nn

