//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_ROOM_H
#define TELNETGAME_ROOM_H


#include <string>
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
    Room(){
        //Todo konstruktor, pobieranie z pliku
    }

    void writeRoomDescription();

    void writeResponses();

    void goToNextRoom(std::string);

};


#endif //TELNETGAME_ROOM_H
