CC=gcc

all : task1 task2
.PHONY : all

task1: task1.c
	$(CC) -Wall task1.c -o task1

task2: task2.c
	$(CC) -Wall task2.c -o task2 -lpthread

.PHONY: clean
clean:
	rm task1 task2
