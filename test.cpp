//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "FileHandler.h"

int main(int argc, char* argv[]){

    for(int i = 0; i < FileHandler::getInstance().getRoomListSize(); i++) {
        FileHandler::getInstance().writeRoom(i);
    }

}