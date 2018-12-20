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

    if(currentRoom->getRoomId().compare("konkurspyt1.loc") == 0 ||
        currentRoom->getRoomId().compare("konkurspyt2.loc") == 0 ||
        currentRoom->getRoomId().compare("konkurspyt3.loc") == 0 ||
        currentRoom->getRoomId().compare("konkurspyt4.loc") == 0 ) {
            std::cout << "Musimy parsowac odpowiedzi!";
        }

    roomId = currentRoom->getNextRoomId(command);

    if(roomId != "Nieprawidlowy wybor. Podaj odpowiedz jeszcze raz")
        {
            std::cout << "room id = " << roomId << "\n";
            currentRoom = FileHandler::getInstance().getNextRoom(roomId);
            std::cout << "currentRoom = " << currentRoom << "\n";
            result = currentRoom->writeRoomDescription();
            std::cout << "result = " << result << "\n";
        }
        else
        {
            result = roomId;
        }
    return result;
}

std::string RoomController::writeCurrentRoomDescription()
{
    return currentRoom->writeRoomDescription();
}
