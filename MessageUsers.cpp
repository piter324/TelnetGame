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

std::string MessageUsers::getRecieverFolderLocation(std::string reciever) {
    std::string messageFileLocation = "../messagesUsers/";
    messageFileLocation += reciever;
    return messageFileLocation;
}


bool MessageUsers::isRecieverInGame(std::string username) {
    //funkcja porownuje nazw lowercase dla wygody uzytkownika
    std::string tmpUsername, tmpLine;
    for(int i = 0; i < username.length(); i++){
        tmpUsername += std::tolower(username[i]);
    }
    std::string usersDataBaseLocation = "../users.db";
    std::ifstream usersDataBaseFile;
    usersDataBaseFile.open(usersDataBaseLocation);

    if(!usersDataBaseFile.is_open()){
        return false;
    }

    std::string line;
    while( std::getline(usersDataBaseFile, line, ':') ){
        for(int i = 0; i < line.length(); i++){
            tmpLine += std::tolower(line[i]);
        }
        if(tmpUsername == tmpLine){
            usersDataBaseFile.close();
            return true;
        }
    }

    return false;
}

Message MessageUsers::newMessage(std::string telnetUsernameMessage) {
    std::stringstream stream(telnetUsernameMessage);
    std::vector<std::string> messageVector;

    //telnet przekazuje string "TELNET LLOGGED_USERNAME RECIEVER MESSAGE" dzielimy go na 3 czesci i tworzymu obiekt message
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

    Message tmpMessage(messageVector[1], messageText);
    tmpMessage.reciever = messageVector[2];
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
    //Kazdy uzytkownik z automatu ma przypisany plik
    //Nalezy sprawdzic czy user o danym nicku istnieje, jezeli nie to znaczy ze nie mozna wyslac wiadomosci
    if(!isRecieverInGame(tmpMessage.reciever)){
        return USER_DOESNT_EXIST;
    }

    std::fstream messageFile;
    messageFile.open(this->getRecieverFolderLocation(tmpMessage.reciever), std::fstream::in | std::fstream::out | std::fstream::app);

    if(!messageFile.is_open()){
        return FILE_NOT_OPENED;
    }

    std::string messageString = tmpMessage.sedner + ";" + tmpMessage.textMessage + "\n";
    messageFile << messageString;
    messageFile.close();
    return SUCCES;
}

int MessageUsers::telnetDeleteMessage(std::string telnetUserAndNumber) {
    //Zakladam, ze funkcja jest wywolywana tylko i wylacznie z wnetrza "okna" pojedynczej wiadomosci
    // albo jest wywolywana automatycznie, albo user ja wywoluje poprzez napisanie jakiegos delete czytajac wiadomosc
    // dalej zakladam, ze sender i numer wiadomosci ( z wywolania instrukcji openSingleMessage ) sa przechowywane
    // i sa przekazywane jako parametr funkcji deleteMessage
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

    //funkcja jest wywolywana z okna pojedynczej wiadomosci, wiec wiadomosc na pewno istnieje

    messages.erase(messages.begin()+messageNumber);

    std::ofstream messageFile;

    messageFile.open(this->getFolderLocation());

    if(!messageFile.is_open()){
        return FILE_NOT_OPENED;
    }

    std::string messageLine;
    for(int i = 0; i < messages.size(); i++){
        messageLine = messages[i].sedner + ";" + messages[i].textMessage + "\n";
        messageFile << messageLine;
    }

    messageFile.close();
    return SUCCES;

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
        return "Nie odnaleziono wiadomosci o danym numerze.\r\n";
    }

    std::string tmpmessage = messages[messageNumber].sedner + " " + messages[messageNumber].textMessage + "\r\n";

    return tmpmessage;
}
