CC=g++ -std=c++11

all: AutorizationController.o AdminController.o FileHandler.o server.o
	$(CC) AutorizationController.o AdminController.o server.o -o server
	rm *.o

server.o: server.cpp
	$(CC) -c server.cpp

AutorizationController.o: AutorizationController.h AutorizationController.cpp
	$(CC) -c AutorizationController.cpp

AdminController.o: AdminController.h AdminController.cpp
	$(CC) -c AdminController.cpp

FileHandler.o: FileHandler.h FileHandler.cpp
	$(CC) -c FileHandler.cpp

# run: server.cpp
# 	$(CC) server.cpp -o server
# 	ip addr | grep "inet "
# 	./server