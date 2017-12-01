all: main.c
	gcc -o main main.c

run: main
	./main
