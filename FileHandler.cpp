//
// Created by Bartosz Cybulski on 12/11/2018.
//


#include "FileHandler.h"

void FileHandler::readFromFile() {
    // czytanie z pliku , funkcja testowa , niepotrzebna do koncowego programu

    std::ifstream locationsFile;

    locationsFile.open(locationLocationsFolder+locationsFileName);

    //std::cout<<locationLocationsFolder+locationsFileName<<"\n";

    if(!locationsFile.is_open()) {
        std::cout<<"Plik nie jest otwarty.\n";
        return;
    }

    std::string line;
    while( std::getline(locationsFile, line) ){
        line = line.substr(0, line.size()-4);
        std::cout<<line<<"\n";
    }

    locationsFile.close();

}

void FileHandler::currentPath() {
    //funkcja testowa , niepotrzebna do koncowego programu
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

int FileHandler::constructRoomList() {

    //Metoda wywolywana przy starcie serwera. Czyta pliki i konstruuje obiekty typu Room i Odpowiedz
    //Zwraca 0 przy pomyslnym utworzeniu obiektow
    //      -1 przy bledzie

    std::ifstream locationsFile;

    locationsFile.open(locationLocationsFolder + locationsFileName);
    if(!locationsFile.is_open()){
        return FAIL;
    }

    std::string fileId;
    while ( std::getline(locationsFile, fileId) ){
        //usuniecie koncowki .loc
        //line = line.substr(0, line.size()-4);

        std::ifstream tmpFile;
        tmpFile.open(locationLocationsFolder + fileId);
        if(!tmpFile.is_open()){
            std::cout<<"blad!! "<<fileId;
            return FAIL;
        }

        //kazda czynnos jest opisana 3 zmiennymi
        // pokoj - roomId, roomHeader, roomDescription
        // odpowiedz - responseId, responseText, responseNextAction
        // po kazdych trzech wykonaniach petli bedze tworzony odpowiedni obiekt
        // dla 1 dzialania pokoj
        // kazdy kolejny utworzy obiekt typu odpowiedz
        std::string line, first_line, second_line, third_line ;
        Room *tmpRoom = nullptr;
        Response *tmpResponse = nullptr;
        int i = 0;
        while(std::getline(tmpFile, line)){
            switch (i % 3){
                case 0: {
                    first_line = line;
                    break;
                }
                case 1:{
                    second_line = line;
                    break;
                }
                case 2:{
                    third_line = line;
                    if(i == 2){
                        tmpRoom = new Room(first_line, second_line, third_line);
                        roomVector.push_back(tmpRoom);
                        roomListSize++;
                    }
                    else{
                        tmpResponse = new Response(first_line, second_line, third_line);
                        tmpRoom->addResponse(tmpResponse);
                    }
                    break;
                }
                default:
                    break;
            }
            i++;
        }
        tmpFile.close();
    }
    locationsFile.close();

    return SUCCESS;
}

Room *FileHandler::getNextRoom(std::string roomID) {

    for(int i = 0 ; i < this->getRoomListSize(); i++){

        if(roomVector[i]->getRoomId() == roomID){
            return roomVector[i];
        }
    }

    return nullptr;
}

Room *FileHandler::getFirstRoom() {
    return getNextRoom(STARTING_POINT);
}
