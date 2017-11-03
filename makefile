all: main.c main.h
	gcc -o wave main.c main.h -pthread
	./wave -N 128 -T 2000 -H 2 -f out.txt -t 500
