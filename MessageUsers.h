//
// Created by Bartosz Cybulski on 17/12/2018.
//

#ifndef TELNETGAME_MESSAGEUSERS_H
#define TELNETGAME_MESSAGEUSERS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


struct Message {
    std::string sedner;
    std::string textMessage;

    Message(std::string sender, std::string message){
        this->sedner = sender;
        this->textMessage = message;
    }

    Message(const std::string &line){

        std::stringstream stream(line);
        std::vector<std::string> messageVector;

        while( stream.good() ){
            std::string tmp;
            getline(stream, tmp, ';');
            messageVector.push_back(tmp);
        }

        this->sedner = messageVector[0];
        this->textMessage = messageVector[1];
    }

    std::string getSenderName() { return sedner ;}
    std::string gettextMessage() { return textMessage; }
};

class MessageUsers {

private:
    std::string username;
    std::vector<Message> messages;

    void setUsername(std::string username) { this->username = username; }

    std::string getUsername() { return this->username; }

    std::string getFolderLocation();

    Message newMessage(std::string telnetUsernameMessage);

    int getAllMessagesFromFile();

public:

    MessageUsers(){}

    std::string telnetSendMessage(std::string revieverUserName, Message message);

    int telnetSendMessage(std::string telnetUsernameMessage);

    void telnetDeleteMessage(std::string username);

    void telnetRespondMessage(std::string username);

    std::string telnetOpenMessages(std::string username);

    std::string telnetOpenSingleMessage(std::string telnetUserAndNumber);

};

//todo usuwanie wiadomosci po odczytaniu, zapisywanie wszystkich wiadomosci do pliku po usuwaniu
#endif //TELNETGAME_MESSAGEUSERS_H
