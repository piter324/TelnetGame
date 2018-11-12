//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_RESPONSE_H
#define TELNETGAME_RESPONSE_H

#include <iostream>
#include <utility>
#include <vector>
#include <string>

/*
 * Klasa Response - klasa akcje (odpowiedzi) możliwe do wykonania w danym pokoju
 */

class Response {

private:
    std::string responseId;
    std::string responseText;
    std::string responseNextAction;


    std::string getResponseId(){
        return responseId;
    }

    std::string getResponseText() {
        return responseText;
    }

    std::string getResponseNextAction(){
        return responseNextAction;
    }


public:
    Response(std::string responseId, std::string responseText, std::string responseNextAction){
        this->responseId            = std::move(responseId);
        this->responseText          = std::move(responseText);
        this->responseNextAction    = std::move(responseNextAction);
    };



    void writeResponse();


};


#endif //TELNETGAME_RESPONSE_H
