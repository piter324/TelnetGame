//
// Created by Bartosz Cybulski on 17/12/2018.
//

#define SUCCES 0
#define FILE_NOT_OPENED -1
#define USER_DOESNT_EXIST -2

#include "MessageUsers.h"

std::string MessageUsers::getFolderLocation() {
    std::string messageFileLocation = "../messagesUsers/";
    messageFileLocation += this->getUsername();
    return messageFileLocation;
}


Message MessageUsers::newMessage(std::string telnetUsernameMessage) {
    std::stringstream stream(telnetUsernameMessage);
    std::vector<std::string> messageVector;

    //telnet przekazuje string "TELNET USERNAME RECIEVER MESSAGE" dzielimy go na 3 czesci i tworzymu obiekt message
    while( stream.good()){
        std::string tmp;
        getline(stream, tmp, ' ');
        messageVector.push_back(tmp);
    }
    setUsername(messageVector[1]);

    std::string messageText;
    for(int i = 3; i < messageVector.size(); i++){
        messageText+=messageVector[i] + " ";
    }
    Message tmpMessage(messageVector[2], messageText);
    return tmpMessage;
}


int MessageUsers::getAllMessagesFromFile() {
    /*Funkcja odczytujaca wszystkie wiadomosci uzytkownika z pliku do pustej struktury*/
    messages.clear();
    std::ifstream messageFile;

    messageFile.open(this->getFolderLocation());

    if(!messageFile.is_open()){
        return FILE_NOT_OPENED;
    }

    std::string line;
    while( std::getline(messageFile, line) ){
        line.size();
        Message tmpMessage(line);
        messages.push_back(tmpMessage);
    }

    messageFile.close();
    return SUCCES;
}

int MessageUsers::telnetSendMessage(std::string telnetUsernameMessage) {
    Message tmpMessage = newMessage(telnetUsernameMessage);
    std::cout<<tmpMessage.sedner<<";"<<tmpMessage.textMessage<<"\n";
    //Kazdy uzytkownik z automatu ma przypisany plik
    //Nalezy sprawdzic czy plik istnieje, jezeli nie to znaczy ze nie mozna wyslac wiadomosci
    //TODO sprawdz czy user istnieje
    std::fstream messageFile;
    messageFile.open(this->getFolderLocation(), std::fstream::in | std::fstream::out | std::fstream::app);

    if(!messageFile.is_open()){
        return FILE_NOT_OPENED;
    }

    std::string messageString = tmpMessage.sedner + ";" + tmpMessage.textMessage + "\n";
    messageFile << messageString;
    messageFile.close();
    return SUCCES;
}

void MessageUsers::telnetDeleteMessage(std::string username) {
    setUsername(username);
}

void MessageUsers::telnetRespondMessage(std::string username) {
    setUsername(username);
}

std::string MessageUsers::telnetOpenMessages(std::string username) {
    /*Funkcja otwierajaca wszystkie wiadomosci*/
    setUsername(username);
    getAllMessagesFromFile();
    std::string allMessages;

    for(int i = 0; i < messages.size(); i++){
        allMessages += "[" + std::to_string(i) + "]. ";
        allMessages += messages[i].sedner;
        allMessages += " ";
        allMessages += messages[i].textMessage;
        allMessages += "\r\n";
    }
    //std::cout<<allMessages;
    return allMessages;
}

std::string MessageUsers::telnetOpenSingleMessage(std::string telnetUserAndNumber) {
    /*Funkcja otwierajaca wiadomosc o podanym numerze*/

    std::stringstream stream(telnetUserAndNumber);
    std::vector<std::string> messageVector;

    //telnet przekazuje string "TELNET USERNAME NUMBER" dzielimy go na 3 czesci i tworzymu obiekt message
    int i = 0;
    while (i < 3) {
        std::string tmp;
        getline(stream, tmp, ' ');
        messageVector.push_back(tmp);
        i++;
    }

    username = messageVector[1];
    setUsername(username);
    int messageNumber = std::stoi(messageVector[2]);

    if(messageNumber >= messages.size() || messageNumber < 0){
        return "Nie odnaleziono wiadomosci o danym numerze";
    }

    std::string tmpmessage = messages[messageNumber].sedner + " " + messages[messageNumber].textMessage;

    return tmpmessage;
}

