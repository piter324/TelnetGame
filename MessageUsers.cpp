//
// Created by Bartosz Cybulski on 17/12/2018.
//

#include "MessageUsers.h"

std::string MessageUsers::getFolderLocation() {
    std::string messageFileLocation = "../messagesUsers/";
    messageFileLocation += this->getUsername();
    return messageFileLocation;
}


Message MessageUsers::newMessage(std::string stringFromTelnet) {
    std::stringstream stream(stringFromTelnet);
    std::vector<std::string> messageVector;

    //telnet przekazuje string "TELNET USERNAME MESSAGE" dzielimy go na 3 czesci i tworzymu obiekt message
    int i = 0;
    while( i < 2){
        std::string tmp;
        getline(stream, tmp, ' ');
        messageVector.push_back(tmp);
        i++;
    }
    Message tmpMessage(messageVector[1], messageVector[2]);
    return tmpMessage;
}


void MessageUsers::getAllMessagesFromFile() {

    std::ifstream messageFile;

    messageFile.open(this->getFolderLocation());

    if(!messageFile.is_open()){
        return;
    }

    std::string line;
    while( std::getline(messageFile, line) ){
        Message tmpMessage(line);
        messages.push_back(tmpMessage);
    }

    messageFile.close();
}

std::string MessageUsers::telnetSendMessage(std::string revieverUserName, Message message) {
    return std::string();
}

void MessageUsers::telnetDeleteMessage(std::string username) {
    setUsername(username);
}

void MessageUsers::telnetRespondMessage(std::string username) {
    setUsername(username);
}

std::string MessageUsers::telnetOpenMessages(std::string username) {
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
    std::stringstream stream(telnetUserAndNumber);
    std::vector<std::string> messageVector;
//todo popraw odczytywanie konkretnej wiadomosci

    //telnet przekazuje string "TELNET USERNAME INT(Number)" dzielimy go na 3 czesci i tworzymu obiekt message
    int i = 0;
    while (i < 2) {
        std::string tmp;
        getline(stream, tmp, ' ');
        messageVector.push_back(tmp);
        i++;
    }
    username = messageVector[1];
    setUsername(username);
    int messageNumber = std::atoi(messageVector[2].c_str());

    std::string tmpmessage = messages[messageNumber].sedner + " " + messages[messageNumber].textMessage;
    std::cout << tmpmessage;

    return tmpmessage;
}

