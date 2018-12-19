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
    //reciever -> w domysle zawsze user u ktorego znajduje sie wiadomosc,
    // potrzebne przy wysylaniu wiadomosci do przekazania parametru
    std::string reciever;
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

};

class MessageUsers {

private:
    std::string username;
    std::vector<Message> messages;

    void setUsername(std::string username) { this->username = username; }

    std::string getUsername() { return this->username; }

    std::string getFolderLocation();

    std::string getRecieverFolderLocation(std::string reciever);

    Message newMessage(std::string username, std::string reciever, std::string textMessage);

    int getAllMessagesFromFile();

    bool isRecieverInGame(std::string username);

public:

    MessageUsers() = default;

    int telnetSendMessage(std::string username, std::string reciever, std::string textMessage);

    int telnetDeleteMessage(std::string username, int number);

    std::string telnetOpenMessages(std::string username);

    std::string telnetOpenSingleMessage(std::string username, int number);

};

#endif //TELNETGAME_MESSAGEUSERS_H
