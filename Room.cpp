//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "Room.h"

#include <sstream>

std::string Room::writeRoomDescription() {
    std::cout<<"CEBUL_writeroomDesctiption()\n";

    std::string returnString = getRoomHeader()  + ".\r\n" + getRoomDescription() + "\r\n" + writeResponses();
    std::cout<<"CEBUL_writeroomDesctiption()\n";
    return returnString;
}

std::string Room::writeResponses() {

    std::string returnString;

    for(int i = 0; i < this->getResponseListSize(); i++){
        Response* tmp = this->getResponse(i);
        returnString += tmp->writeResponse();
    }

    return returnString;
}

std::string Room::getNextRoomId(std::string answerLine) {

    char answerId = answerLine[0];
    answerId = (char) toupper(answerId);

    int answerIdNumber = (int) answerId - 64;

    if(answerIdNumber <= getResponseListSize()){
        std::cout << "DOSZLISMY TUTAJ1\n";
        std::string responseNextAction = responseList[answerIdNumber - 1]->getResponseNextAction(); //wylatuje poza zakres?
        std::cout << "UMIEM\n";
        if(responseNextAction.find('#') == std::string::npos) {
             std::cout << "UPS\n";
            return responseNextAction;
        }
        else{
            std::string answerToQuestion = answerLine.erase(0, 2);

            std::stringstream stream(responseNextAction);
            std::vector<std::string> responseVector;

            std::cout << "DOSZLISMY TUTAJ2\n";
            //jezeli odpowiedz ma format type#CORRECTANSWER#GO_TO_CORRECT#GO_TO_INCORRECT
            std::string tmp;
            while ( stream.good()) {
                getline(stream, tmp, '#');
                responseVector.push_back(tmp);
            }

            std::string corrrectAnswer = responseVector[1];
            std::string correctLoc = responseVector[2];
            std::string incorrectLoc = responseVector[3];
            std::cout << "DOSZLISMY TUTAJ3\n";
            //if(answerToQuestion == corrrectAnswer){ 
            if(answerToQuestion.compare(corrrectAnswer)== 0){
                return correctLoc;
            }
            else
                return incorrectLoc;
        }
    }
    else
        return "Nieprawidlowa odpowiedz. Podaj odpowiedz jeszcze raz";


}

