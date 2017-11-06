all: main.c main.h
	gcc -o wave main.c main.h -pthread
	./wave -N 9 -T 2000 -H 4 -f out.txt -t 500
