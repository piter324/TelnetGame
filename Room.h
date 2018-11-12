//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_ROOM_H
#define TELNETGAME_ROOM_H


#include <string>
#include <fstream>
#include <utility>

#include "Response.h"

class Room {

private:
    std::string roomId;
    std::string roomHeader;
    std::string roomDescription;
    std::vector<Response*> responseList;
    int responseListSize;

    std::string getRoomId(){
        return roomId;
    }

    std::string getRoomHeader(){
        return roomHeader;
    }

    std::string getRoomDescription(){
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

    void addResponse(Response* response){
        responseList.push_back(response);
        responseListSize++;
    }

    void writeRoomDescription();

    void writeResponses();

    void goToNextRoom(std::string);//todo chodzenie po pokojach

};


#endif //TELNETGAME_ROOM_H
