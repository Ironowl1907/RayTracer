CC =g++
CFLAGS =-std=c++11
EXECNAME = app

all: main

main.o: src/main.cpp
	$(CC) -c -o $@ $^ $(CFLAGS)

main: main.o
	$(CC) -o $(EXECNAME)  $^  $(CFLAGS)
