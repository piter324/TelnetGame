#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h> //inet_ntoa
#include <unistd.h> //fork

#define SERVER_PORT 3130
#define QUEUE_SIZE 16
#define BUFFER_SIZE 256

int socketFd;

void interrupt_handler(int signalNum){
    printf("SIGNAL: %d, Server on desc: %d is closing...\n", signalNum, socketFd);
    close(socketFd);
    exit(0);
}

void close_parent_swap_for_child(int clientSocketFd){
    printf("Closing: %d - client: %d\n", socketFd, clientSocketFd);
    close(socketFd);
    socketFd = clientSocketFd;
    printf("%d - %d\n", socketFd, clientSocketFd);
}

int main(int argc, char* argv[]){
    signal(SIGINT, &interrupt_handler);
    printf("\n\n\nSecrets of Elka\n--------------------\n");

    // Create file for socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFd < 0){
        printf("Socket call error\n");
        return 1;
    }

    // Bind socket to local address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    int nBind = bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
    if(nBind < 0) {
        printf("Can't bind a name to a socket\n");
        return 1;
    }

    // Create a queue for incoming connections
    int nListen = listen(socketFd, QUEUE_SIZE);
    if(nListen < 0){
        printf("Can't create an incoming queue\n");
        return 1;
    }

    int message_index = 0;
    char message[BUFFER_SIZE];
    bzero(message, BUFFER_SIZE);

    printf("Server starts listening on port %d...\n", SERVER_PORT);
    while(1){

        // Accept connection from client and return new socket file descriptor for this particular client.
        struct sockaddr_in clientAddress;
        socklen_t nTmp = sizeof(struct sockaddr);
        int clientSocketFd = accept(socketFd, (struct sockaddr*)&clientAddress, &nTmp);
        if(clientSocketFd < 0){
            printf("Can't connect to client\n");
        }
        // Create new process just for this client
        if(fork() == 0){
            close_parent_swap_for_child(clientSocketFd); // Close not fully associated parent socket
            printf("Connection from %s on port %d is open\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
            char buffer[BUFFER_SIZE]; // buffer for incoming messages
            

            while(1) {
                bzero(buffer, BUFFER_SIZE); // zero buffer out
                // strcpy(buffer,"\e[2J >"); // 
                strcpy(buffer,"> "); // 
                send(clientSocketFd, buffer, BUFFER_SIZE, 0);

                while(1){
                    int bytesRead = read(clientSocketFd, buffer, BUFFER_SIZE);
                    if(bytesRead < 1){ //cliens has lost connection
                        printf("Client %s disconnected\n", inet_ntoa(clientAddress.sin_addr));
                        close(clientSocketFd);
                        return 0;
                    }
                    for(int i = 0; i<bytesRead; i++){
                        if((int)buffer[i] == 8) { //backspace pressed
                            if(message_index>0) message_index--;
                        }
                        else {
                            message[message_index++] = buffer[i];
                            printf("%d\n", (int)buffer[i]);
                            if(buffer[i] == '\n') break;
                        }
                    }
                    if(message[message_index-1] == '\n') {
                        printf("Message: %s\n", message);
                        bzero(buffer, BUFFER_SIZE);
                        break;
                    }
                }
                message_index = 0;

                if(strcmp(message, "joke\r\n") == 0){
                    strcpy(buffer, "What color is the mailbox inside?\r\n– Infrared.\r\n");
                    send(clientSocketFd, buffer, BUFFER_SIZE, 0);
                }
               
                if(strcmp(message, "exit\r\n") == 0) {
                    printf("Connection from %s is on port %d closing\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                    close(clientSocketFd);
                    return 0;
                }
                bzero(message, BUFFER_SIZE);
            }
        }
        close(clientSocketFd); // Close child socket - served by forked process
    }

    
}