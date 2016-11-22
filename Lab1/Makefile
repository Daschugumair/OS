CC=gcc

all : task1 task2 task3 task4 task5
.PHONY : all

task1: task1.c
	$(CC) -Wall task1.c -o task1

task2: task2.c
	$(CC) -Wall task2.c -o task2 -lpthread

task3: task3.c
	$(CC) -Wall task3.c -o task3

task4: task4.c
	$(CC) -Wall task4.c -o task4 -lrt

task5: task5.c
	$(CC) -Wall task5.c -o task5 -lrt

.PHONY: clean
clean:
	rm -vf task1 task2 task3 task4 task5
