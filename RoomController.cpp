//
// Created by Bartlomiej Jurek on 17/12/2018.
//

#include "RoomController.h"

std::string RoomController::run()
{
    currentRoom = FileHandler::getInstance().getFirstRoom();
    return currentRoom->writeRoomDescription();
}

std::string RoomController::request(std::string command)
{
    std::string result;
    std::string roomId;

    roomId = currentRoom->getNextRoomId(command);

    if(roomId != "Nieprawidlowa odpowiedz. Podaj odpowiedz jeszcze raz")
        {
            currentRoom = FileHandler::getInstance().getNextRoom(roomId);
            result = currentRoom->writeRoomDescription();
        }
        else
        {
            result = roomId;
        }
    return result;
}
