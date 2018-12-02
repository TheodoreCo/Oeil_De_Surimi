all : main

main : main.o img_treatment.o
	gcc -o ImageSegmentation main.o img_treatment.o -lm

main.o : main.c ../img_treatment.h
	gcc -o main.o -c main.c

img_treatment.o : ../img_treatment.c
	gcc -o img_treatment.o -c ../img_treatment.c
