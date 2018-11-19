#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h> //inet_ntoa
#include <unistd.h> //fork

#define SERVER_PORT 3130
#define QUEUE_SIZE 16
#define BUFFER_SIZE 10

void graceful_TCP_shutdown();
void close_parent_swap_for_child(int clientSocketFd);
void send_message(std::string message, bool prompt);

int socketFd;
bool child = false;

void interrupt_handler(int signalNum){
    printf("SIGNAL: %d, Server on desc: %d is closing...\n", signalNum, socketFd);
    graceful_TCP_shutdown();
    exit(0);
}

void graceful_TCP_shutdown(){
    if(child) {
        printf("--- Child shutdown started\n");
        shutdown(socketFd, 1);
        char buffer[BUFFER_SIZE];
        while(read(socketFd, buffer, BUFFER_SIZE) > 0);
    }
    close(socketFd);
    if(child) printf("--- Child shutdown completed\n");
}

void close_parent_swap_for_child(int clientSocketFd){
    printf("Closing: %d - client: %d\n", socketFd, clientSocketFd);
    close(socketFd);
    child = true;
    socketFd = clientSocketFd;
    printf("%d - %d\n", socketFd, clientSocketFd);
}

void send_message(std::string message, bool prompt = true){
    int charsToSend = message.length();
    std::string toSend;

    while(charsToSend > 0){
        // printf("Chars left: %d\n", charsToSend);

        if(charsToSend > BUFFER_SIZE){
            toSend = message.substr(message.length() - charsToSend, BUFFER_SIZE);
            charsToSend -= BUFFER_SIZE;
        } else {
            toSend = message.substr(message.length() - charsToSend);
            charsToSend = 0;
        }
        send(socketFd, toSend.c_str(), toSend.length(), 0);
    }

    if(prompt){
        toSend = "\r\n> ";
        send(socketFd, toSend.c_str(), toSend.length(), 0);
    }
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
    int yes = 1;
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) < 0){ // setting options to reuse port
        printf("Error setting options for socket");
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
            
            while(1) { // client event loop

                int message_index = 0;
                std::string message = "";

                char buffer[BUFFER_SIZE]; // buffer for incoming messages
                for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0'; // zero buffer out

                send_message("Welcome to Secrets of Elka!\r\nFirst, you need to know some rules:\r\n 1.");
                while(1){
                    int bytesRead = read(clientSocketFd, buffer, BUFFER_SIZE);
                    if(bytesRead < 1){ //cliens has lost connection
                        printf("Client %s disconnected\n", inet_ntoa(clientAddress.sin_addr));
                        graceful_TCP_shutdown();
                        return 0;
                    }
                    for(int i = 0; i<bytesRead; i++){
                        if((int)buffer[i] == 8) { //backspace pressed
                            if(message_index>0) {
                                message_index--;
                                message.pop_back();
                                send_message("\e[K", false); // send command to remove character after backspace
                            }
                            else {
                                send_message("\e[2D> ",false); // send command to move telnet cursor and display prompt sign to keep it intact when pressing backspace
                            }
                        }
                        else if((int)buffer[i] == 3) { // Ctrl+C pressed
                            printf("Connection from %s is on port %d closing due to sent Ctrl+C\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                            graceful_TCP_shutdown();
                            return 0;
                        }
                        else if((int)buffer[i] == '\n' || (int)buffer[i] == '\r' || ((int)buffer[i] >= 32 && (int)buffer[i] <= 126)) { //accepted characters
                            message_index++;
                            message.push_back(buffer[i]);
                            // printf("%d\n", (int)buffer[i]); // print the character id
                            if((int)buffer[i] == '\n') break;
                        }
                    }
                    
                    if(message[message.length()-1] == '\n') {
                        printf("Message from %s:%d : %s\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), message.c_str());
                        break;
                    }
                }
                message_index = 0;
                
                if(message.compare("joke\r\n") == 0){
                    send_message("What color is the mailbox inside?\r\n– Infrared.\r\n");
                }
               
                if(message.compare("exit\r\n") == 0) {
                    printf("Connection from %s is on port %d closing\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                    graceful_TCP_shutdown();
                    return 0;
                }
            }
        }
        close(clientSocketFd); // Close child socket - served by forked process
    }

    
}