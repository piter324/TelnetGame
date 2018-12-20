//
// Created by Bartlomiej Jurek on 17/12/2018.
//

#ifndef ROOM_CONTROLLER_H
#define ROOM_CONTROLLER_H

#include "FileHandler.h"

class RoomController
{
    Room* currentRoom;

public:

    std::string run();
    std::string request(std::string command);
    std::string writeCurrentRoomDescription();
};


#endif // ROOM_CONTROLLER_H
