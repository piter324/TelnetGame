#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <arpa/inet.h> //inet_ntoa
#include <unistd.h> //fork
#include <sstream>
#include <string>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>


#include "AutorizationController.h"
#include "AdminController.h"
#include "RoomController.h"
#include "MessageController.h"

#define SERVER_PORT 3130
#define QUEUE_SIZE 16
#define BUFFER_SIZE 10

void graceful_TCP_shutdown();
void close_parent_swap_for_child(int clientSocketFd);
void send_message(std::string message, bool prompt);

int socketFd;
bool child = false;
sem_t* semUsers;

void interrupt_handler(int signalNum){
    AdminController adminController;
    adminController.request("restart");
    sem_unlink ("semUsers");
    sem_close(semUsers);
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
    // printf("Closing: %d - client: %d\n", socketFd, clientSocketFd);
    close(socketFd);
    child = true;
    socketFd = clientSocketFd;
    // printf("%d - %d\n", socketFd, clientSocketFd);
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
    semUsers = sem_open("semUsers",  O_CREAT | O_EXCL, 0644, 1);
    if(semUsers==nullptr)
    {
        printf("Semaphore \"semUsers\"already exists.");
        exit(1);
    }
    system("mkdir loggedUsers");
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

            #define WELCOME 0
            #define LOG_IN_UNAME 1
            #define LOG_IN_PASS 2
            #define BACK_TO_WELCOME 3
            #define ROOM_NAV 4
            #define ADMIN_PASS 5
            #define REGISTER_UNAME 6
            #define REGISTER_PASS 7
            #define ADMIN_SESSION 8
            #define MAILBOX_STATE 9

            std::string uname, pass;
            std::string tmp;
            int state = WELCOME; // current state of a server
            AutorizationController auth = AutorizationController();
            AdminController admin = AdminController();
            RoomController roomc = RoomController();
            MessageController messageController;

            while(1) { // client event loop

                int message_index = 0;
                std::string message = "";

                char buffer[BUFFER_SIZE]; // buffer for incoming messages
                for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0'; // zero buffer out

                switch(state) { // send message to user
                    case WELCOME:
                        send_message("\r\nWitamy w 'Pogoni za A+'!\r\nNajpierw kilka zasad:\r\n 1. Komendy dostepne globalnie to: exit - aby wyjsc z serwera, \r\n a po zalogowaniu takze:\r\n send <nazwa odbiorcy> <wiadomosc> - wysylanie wiadomosci, \r\n mailbox - wyswietlenie swojej skrzynki wiadomosci, \r\n show active - wyswietlenie listy aktywnych uzytkownikow \r\n 2. W nawiasach kwardratowych napisalismy komende (lub litere), ktora nalezy wpisac, aby wywolac przypisana do niej akcje\r\n 3. W niektorych lokalizacjach format odpowiedzi bedzie nieco inny, ale poinformujemy o tym po wejsciu do tej lokalizacji\r\n Co chcesz zrobic: \r\n[login] Zaloguj sie\r\n[register] Zarejestruj sie\r\n[admin] Rozpocznij sesje admina\r\n[exit] Wyjdz\r\n");
                        break;
                    case BACK_TO_WELCOME:
                        send_message("Co chcesz zrobic?\r\n[back] Wroc\r\n[exit] Wyjdz\r\n");
                        break;
                }
                while(1){
                    int bytesRead = read(clientSocketFd, buffer, BUFFER_SIZE);
                    if(bytesRead < 1){ //cliens has lost connection
                        printf("Client %s on %s disconnected. Logging out\r\n", uname.c_str(), inet_ntoa(clientAddress.sin_addr));
                        if(uname != "") auth.logOut(uname);
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
                            printf("Connection from %s is on port %d closing due to sent Ctrl+C\r\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                            if(uname != "") auth.logOut(uname);
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
                        printf("Message from %s:%d : %s\r\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), message.c_str());
                        break;
                    }
                }
                if(message.length()==0) continue;
                message_index = 0;
                message = message.substr(0, message.size()-2);
                // printf("%s\n", message);

                if(message == "joke"){
                    send_message("What color is the mailbox inside?\r\n> Infrared.\r\n");
                }

                if(message == "help"){
                    send_message(" 1. Komendy dostepne globalnie to: exit - aby wyjsc z serwera, \r\n send <nazwa odbiorcy> <wiadomosc> - wysylanie wiadomosci, \r\n mailbox - wyswietlenie swojej skrzynki wiadomosci, \r\n show active - wyswietlenie listy aktywnych uzytkownikow \r\n 2. W nawiasach kwardratowych napisalismy komende (lub litere), ktora nalezy wpisac, aby wywolac przypisana do niej akcje\r\n 3. W niektorych lokalizacjach format odpowiedzi bedzie nieco inny, ale poinformujemy o tym po wejsciu do tej lokalizacji\r\n");
                }

                if(message == "exit") {
                    printf("Connection from %s is on port %d closing\r\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
                    if(uname != "") auth.logOut(uname);
                    graceful_TCP_shutdown();
                    return 0;
                }

                if(state == ROOM_NAV || state == MAILBOX_STATE) {
                    if(!auth.isLoggedIn(uname)){
                        send_message("------- Zostales wylogowany z serwera -------\r\n");
                        state = WELCOME;
                        continue;
                    }
                }

                switch(state) { // send message to user
                    case WELCOME:
                        if(message == "login") {
                            state = LOG_IN_UNAME;
                            send_message("Podaj swoja nazwe uzytkownika:");
                        }
                        else if(message == "register") {
                            state = REGISTER_UNAME;
                            send_message("Podaj nazwe nowego uzytkownika: ");
                        }
                        else if(message == "admin") {
                            state = ADMIN_PASS;
                            send_message("Podaj haslo administratora: ");
                        }
                        break;

                    case LOG_IN_UNAME:
                        uname = message;
                        state = LOG_IN_PASS;
                        send_message("Podaj swoje haslo: ");
                        break;

                    case LOG_IN_PASS:
                        pass = message;
                        if(auth.logIn(uname, pass)) {
                            send_message("Zalogowanie przebieglo pomyslnie\r\n", false);
                            send_message(roomc.writeNumberOfMessages(uname));
                            send_message(roomc.run());
                            state = ROOM_NAV;
                        } else {
                            send_message("Blad logowania.\r\n", false);
                            state = BACK_TO_WELCOME;
                            uname = "";
                            pass = "";
                        }
                        break;

                    case BACK_TO_WELCOME:
                        if(message == "back") state = WELCOME;
                        break;

                    case REGISTER_UNAME:
                        uname = message;
                        state = REGISTER_PASS;
                        send_message("Podaj haslo nowego uzytkownika:");
                        break;

                    case REGISTER_PASS:
                        pass = message;
                        state = BACK_TO_WELCOME;
                        if(auth.registerUser(uname, pass)) {
                            send_message("Pomyslnie zarejestrowano uzytkownika\r\n", false);
                        } else {
                            send_message("Rejestracja nieudana\r\n", false);
                        }
                        uname = "";
                        pass = "";
                        break;


                    case ADMIN_PASS:
                        if(message == "admin") { // admin password correct
                            state = ADMIN_SESSION;
                            send_message(admin.request("help"));

                        } else {
                            send_message("Niepoprawne haslo administratora\r\n", false);
                            state = BACK_TO_WELCOME;
                        }
                        break;

                    case ADMIN_SESSION:
                        send_message(admin.request(message));
                        break;

                    case ROOM_NAV:
                        std::getline(std::stringstream(message), tmp, ' ');
                        if(tmp=="send")
                            send_message(messageController.send(uname + " " + message));
                        else if(tmp=="mailbox")
                        {
                            state = MAILBOX_STATE;
                            send_message("----------------------\r\nWelcome to MAILBOX!\r\n");
                            send_message(roomc.writeNumberOfMessages(uname));
                            send_message(messageController.help());
                        }
                        else if (message == "show active") {
                            send_message(admin.request("show active"));
                        }
                        else {
                            send_message(roomc.writeNumberOfMessages(uname));
                            send_message(roomc.request(message));
                        }
                        break;

                    case MAILBOX_STATE:
                        if(message == "back")
                        {
                             state = ROOM_NAV;
                             send_message(roomc.writeNumberOfMessages(uname));
                             send_message(roomc.writeCurrentRoomDescription());
                        }
                        else if (message == "show active") {
                            send_message(admin.request("show active"));
                        }
                        else
                            send_message(messageController.request(uname + " " + message));
                        break;
                }


            }
        }
        close(clientSocketFd); // Close child socket - served by forked process
    }
}
