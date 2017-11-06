all: main.c main.h
	gcc -o wave main.c main.h -pthread
	./wave -N 10 -T 15 -H 4 -f out.txt -t 500
