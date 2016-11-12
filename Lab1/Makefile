CC=gcc

all : task1 task2 task3
.PHONY : all

task1: task1.c
	$(CC) -Wall task1.c -o task1

task2: task2.c
	$(CC) -Wall task2.c -o task2 -lpthread

task3: task3.c
	$(CC) -Wall task3.c -o task3

.PHONY: clean
clean:
	rm -vf task1 task2 task3
