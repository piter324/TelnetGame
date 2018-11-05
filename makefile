CC=g++

all: server.cpp
	$(CC) server.cpp -o server

run: server.cpp
	$(CC) server.cpp -o server
	ip addr | grep "inet "
	./server