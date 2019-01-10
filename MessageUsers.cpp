//
// Created by Bartosz Cybulski on 17/12/2018.
//

#define SUCCES 0
#define FILE_NOT_OPENED -1
#define USER_DOESNT_EXIST -2

#include "MessageUsers.h"

std::string MessageUsers::getFolderLocation() {
    std::string messageFileLocation = "messagesUsers/";
    messageFileLocation += this->getUsername();
    return messageFileLocation;
}

std::string MessageUsers::getRecieverFolderLocation(std::string reciever) {
    std::string messageFileLocation = "messagesUsers/";
    messageFileLocation += reciever;
    return messageFileLocation;
}


bool MessageUsers::isRecieverInGame(std::string username) {
    //funkcja porownuje nazw lowercase dla wygody uzytkownika
    std::string tmpUsername, tmpLine;
    for(int i = 0; i < username.length(); i++){
        tmpUsername += std::tolower(username[i]);
    }
    std::string usersDataBaseLocation = "users.db";
    std::ifstream usersDataBaseFile;
    usersDataBaseFile.open(usersDataBaseLocation);

    if(!usersDataBaseFile.is_open()){
        return false;
    }

    std::string line;
    while( std::getline(usersDataBaseFile, line) ){
        tmpLine = "";
        for(int i = 0; i < line.length(); i++){
            if(line[i] == ':'){
                break;
            }
            tmpLine += std::tolower(line[i]);
        }
        if(tmpUsername == tmpLine){
            usersDataBaseFile.close();
            return true;
        }
    }
    usersDataBaseFile.close();
    return false;
}

Message MessageUsers::newMessage(std::string username, std::string reciever, std::string textMessage) {

    setUsername(username);


    Message tmpMessage(username, textMessage);
    tmpMessage.reciever = reciever;
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
        numberOfMessages++;
    }

    messageFile.close();
    return SUCCES;
}

int MessageUsers::telnetSendMessage(std::string username, std::string reciever, std::string textMessage) {
    Message tmpMessage = newMessage(move(username), move(reciever), move(textMessage));
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

std::string MessageUsers::telnetOpenSingleMessage(std::string username, int number) {
/*Funkcja otwierajaca wiadomosc o podanym numerze*/


    setUsername(std::move(username));
    getAllMessagesFromFile();
    int messageNumber = number - 1;

    if(messageNumber >= messages.size() || messageNumber < 0){
        return "Nie odnaleziono wiadomosci o danym numerze.\r\n";
    }

    std::string tmpmessage = messages[messageNumber].sedner + " " + messages[messageNumber].textMessage + "\r\n";

    return tmpmessage;
}

int MessageUsers::telnetDeleteMessage(std::string username, int number) {
    //Zakladam, ze funkcja jest wywolywana tylko i wylacznie z wnetrza "okna" pojedynczej wiadomosci
    // albo jest wywolywana automatycznie, albo user ja wywoluje poprzez napisanie jakiegos delete czytajac wiadomosc
    // dalej zakladam, ze sender i numer wiadomosci ( z wywolania instrukcji openSingleMessage ) sa przechowywane
    // i sa przekazywane jako parametr funkcji deleteMessage

    setUsername(move(username));
    getAllMessagesFromFile();
    int messageNumber = number - 1;

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
        allMessages += "[" + std::to_string(i + 1) + "]. ";
        allMessages += messages[i].sedner;
        allMessages += " ";
        allMessages += messages[i].textMessage;
        allMessages += "\n";
    }
    //std::cout<<allMessages;
    return allMessages;
    //todo zwroc jakis string ze nie ma wiadomosci albo cos...
}

int MessageUsers::telnetNumberOfMessages(std::string username) {
    setUsername(username);
    getAllMessagesFromFile();
    return numberOfMessages;
}
