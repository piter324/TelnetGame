//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_ROOM_H
#define TELNETGAME_ROOM_H


#include <string>
#include <fstream>
#include <utility>
#include <algorithm> //for std::transform

#include "Response.h"

class Room {

private:
    std::string roomId;
    std::string roomHeader;
    std::string roomDescription;
    std::vector<Response*> responseList;
    int responseListSize;


    std::string getRoomHeader(){
        std::cout<<"CEBUL_getROOMHEADER()=="<<roomHeader<<"\n";
        return roomHeader;
    }

    std::string getRoomDescription(){
        std::cout<<"CEBUL_getROOMDescription()=="<<roomDescription<<"\n";
        return roomDescription;
    }

    int getResponseListSize(){
        return responseListSize;
    }

    Response* getResponse(int responseNumber){
        return responseList[responseNumber];
    }


public:
    Room(std::string roomId, std::string roomHeader, std::string roomDescription){
        this->roomId            = std::move(roomId);
        this->roomHeader        = std::move(roomHeader);
        this->roomDescription   = std::move(roomDescription);
        this->responseListSize  = 0;
    }

    std::string getRoomId(){
        return roomId;
    }

    void addResponse(Response* response){
        responseList.push_back(response);
        responseListSize++;
    }

    std::string writeRoomDescription();

    std::string writeResponses();

    //funkcja ktora przyjmuje jako parametr ID odpowiedzi ( A/B/C ) i podaje nazwe kolejnego pliku
    //poniewaz telnet przesyla cale linijki przyjmujemy od uzytkownika String i ucinamy do pierwszej litery

    std::string getNextRoomId(std::string);

};


#endif //TELNETGAME_ROOM_H
