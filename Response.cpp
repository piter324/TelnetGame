//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "Response.h"


void Response::writeResponse() {

        std::cout<<"["<<this->getResponseId()<<"]. "<<this->getResponseText()<<".\n";
}


