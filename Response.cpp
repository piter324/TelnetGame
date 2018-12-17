//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "Response.h"


std::string Response::writeResponse() {

        std::cout<<"["<<this->getResponseId()<<"]. "<<this->getResponseText()<<".\n";
        std::string returnString = "["+this->getResponseId() + "]. " + this->getResponseText() + ".\r\n";
        return returnString;
}


