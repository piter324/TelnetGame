//
// Created by Bartosz Cybulski on 12/11/2018.
//

#ifndef TELNETGAME_FILEHANDLER_H
#define TELNETGAME_FILEHANDLER_H

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "Room.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class FileHandler {

private:

    const std::string locationsFileName = "locations.list";
    const std::string locationLocationsFolder = "../locations/";
    std::vector<Room*> roomList;
    std::ifstream locationsFile;

public:
    FileHandler(){
    }

    void readFromFile(){
        // czytanie z pliku
        locationsFile.open(locationLocationsFolder+locationsFileName);

        std::cout<<locationLocationsFolder+locationsFileName<<"\n";

        if(!locationsFile.is_open()) {
            std::cout<<"Plik nie jest otwarty.\n";
            return;
        }

        std::string line;
        while( std::getline(locationsFile, line) ){

            std::cout<<line<<"\n";
        }


        locationsFile.close();
    }

    void currentPath(){
        char cCurrentPath[FILENAME_MAX];

        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            //return errno;
            return;
        }

        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

        printf ("The current working directory is %s", cCurrentPath);
        //return cCurrentPath;
    }

};

//todo do as singleton

#endif //TELNETGAME_FILEHANDLER_H
