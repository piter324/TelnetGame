//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_FILEHANDLER_H
#define TELNETGAME_FILEHANDLER_H

#include <cstdio>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define SUCCESS 0
#define FAIL (-1)
#define STARTING_POINT "poczatek.loc"

#include "Room.h"

#include <fstream>
#include <iostream>
#include <string>

//FileHandler - klasa realizowana jako singleton. Wywoływana na początku uruchomienia programu czyta pliki i tworzy instancje klas pokoj oraz odpowiedz, jednocześnie tablice wskaznikow na obiekty Pokoj

class FileHandler {

private:

    const std::string locationsFileName = "locations.list";
    const std::string locationLocationsFolder = "../locations/";
    std::vector<Room*> roomVector;
    int roomListSize;

    FileHandler() {
        this->roomListSize = 0;
        constructRoomList();
        // TODO co jezeli nie uda sie odczytac plikow
    }
    FileHandler(const FileHandler &);
    FileHandler& operator=(const FileHandler&);
    ~FileHandler() = default;

    int constructRoomList() ;

public:
    static FileHandler& getInstance(){
        // Inicjalizacja statycznego obiektu.
        // Obiekt zostanie utworzony przy pierwszym wywołaniu tej metody
        // i tylko wtedy nastąpi inicjalizacja przy pomocy konstruktora. //Każde następne wywołanie jedynie zwróci referencję tego obiektu.
        // wywolanie FileHandler::getInstance().method();

        static FileHandler instance;
        return instance;
    }

    void readFromFile();

    void currentPath();

    void writeRoom(int i){
        roomVector[i]->writeRoomDescription();
    }

    int getRoomListSize(){ return this->roomListSize ;}

    //zawsze zaczynamy gre od pliku poczatek.loc
    Room* getFirstRoom();

    //znając ID pokoju ( nazwe pliku ) wybieramy kolejny pokoj
    Room* getNextRoom (std::string roomID);
};


#endif //TELNETGAME_FILEHANDLER_H
