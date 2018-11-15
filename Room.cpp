//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "Room.h"

void Room::writeRoomDescription() {

    std::cout<<this->getRoomHeader()<<".\n"<<this->getRoomDescription()<<"\n";
    this->writeResponses();
    std::cout<<"\r\n";
}

void Room::writeResponses() {

    for(int i = 0; i < this->getResponseListSize(); i++){
        Response* tmp = this->getResponse(i);
        tmp->writeResponse();
    }

}

void Room::goToNextRoom(std::string) {
    //todo przejscie do nastepnej lokacji
}
