CC=g++ -std=c++11 -pthread

all: AutorizationController.o AdminController.o Response.o Room.o FileHandler.o RoomController.o MessageUsers.o MessageController.o server.o
	$(CC) AutorizationController.o AdminController.o Response.o Room.o FileHandler.o RoomController.o MessageUsers.o MessageController.o server.o -o server
	rm *.o

server.o: server.cpp
	$(CC) -c server.cpp

AutorizationController.o: AutorizationController.h AutorizationController.cpp
	$(CC) -c AutorizationController.cpp

AdminController.o: AdminController.h AdminController.cpp
	$(CC) -c AdminController.cpp

RoomController.o: RoomController.h RoomController.cpp
	$(CC) -c RoomController.cpp

FileHandler.o: FileHandler.h FileHandler.cpp
	$(CC) -c FileHandler.cpp

Room.o: Room.h Room.cpp
	$(CC) -c Room.cpp

Response.o: Response.h Response.cpp
	$(CC) -c Response.cpp

MessageUsers.o: MessageUsers.h MessageUsers.cpp
	$(CC) -c MessageUsers.cpp

MessageController.o: MessageController.h MessageController.cpp
	$(CC) -c MessageController.cpp

# run: server.cpp
# 	$(CC) server.cpp -o server
# 	ip addr | grep "inet "
# 	./server
