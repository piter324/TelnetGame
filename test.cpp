//
// Created by Bartosz Cybulski on 12/11/2018.
//

#include "FileHandler.h"
#include "MessageUsers.h"

int main(int argc, char* argv[]){

//    for(int i = 0; i < FileHandler::getInstance().getRoomListSize(); i++) {
//        FileHandler::getInstance().writeRoom(i);
//    }

/********** TEST CHODZENIA PO POKOJACH **********/
/*    Room* currentRoom = FileHandler::getInstance().getFirstRoom();
    currentRoom->writeRoomDescription();

    std::string response , roomId;

    while(true){
        std::cout<< "Co wybierasz ?"<<std::endl;
        std::cin>>response;

        std::system(CLEAR);

        roomId = currentRoom->getNextRoomId(response);

        if(roomId != "Nieprawidlowa odpowiedz. Podaj odpowiedz jeszcze raz") {
            currentRoom = FileHandler::getInstance().getNextRoom(roomId);
            currentRoom->writeRoomDescription();
        }
        else {
            std::cout << roomId << std::endl;
            currentRoom->writeResponses();
        }
    }*/

    /********* TEST WIADOMOSCI **********/

    MessageUsers messages;
    messages.telnetOpenMessages("Ala");
    std::cout<<messages.telnetOpenMessages("Ala");
    std::cout<<messages.telnetOpenSingleMessage("tel Ala 3");
    messages.telnetDeleteMessage("tel Ala 0");
    std::cout<<messages.telnetOpenMessages("Ala");

    //std::cout<<messages.telnetSendMessage("tel bcybulsk Ala wiadomosc do Ala.");
    return 0;
}
