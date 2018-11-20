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

std::string Room::getNextRoomId(std::string answerLine) {

    char answerId = answerLine[0];
    answerId = (char) toupper(answerId);

    int answerIdNumber = (int) answerId - 64;

    if(answerIdNumber <= getResponseListSize()){
        return responseList[answerIdNumber - 1]->getResponseNextAction();
    }
    else
        return "Nieprawidlowa odpowiedz. Podaj odpowiedz jeszcze raz";


}

