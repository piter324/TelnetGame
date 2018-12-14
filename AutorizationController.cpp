

#include "AutorizationController.h"

/* checks the existence of the user file */ 
bool AutorizationController::isLoggedIn(std::string username) {
    std::cout << "Uruchamiamy funkcje isLoggedIn z param " << username << std::endl;
    std::ifstream file;
    file.open(pathToUserFolder_ + username);
    if (file){
        file.close();
        std::cout << "Plik istnije " <<  std::endl;
        return true;
    }
    file.close();
    return false;
}

bool AutorizationController::logIn(std::string username, std::string passw) {
    std::cout << "Uruchamiamy funkcje logIn z param " << username << std::endl;
    std::ifstream dbFile;
    dbFile.open(pathToUserDatabase_);
    bool userFound = false;
    if (dbFile) {
        std::string userInfo;
        while (!userFound && getline (dbFile,userInfo) ) {
           // std::cout << "szukamy... " << userInfo.length() <<" " << username.length() << " " << passw.length() << std::endl;
            if(userInfo.length() == username.length() + passw.length() + 2) {
                std::cout << "kandydat... " << std::endl;
                std::size_t posUsername = userInfo.find(username);
                std::size_t posPasswd = userInfo.find(passw);
                if(posUsername != std::string::npos && posPasswd != std::string::npos) {
                    if(posUsername == 0 && posPasswd == username.length() + 1)
                        userFound = true;
                }
            }
        }
        dbFile.close();
        if(userFound) {
            std::cout << "tworzymy plik gracza" << std::endl;
            std::ofstream userFile(pathToUserFolder_ + username);
            userFile.close();
            return true;
        }
            
    }
    return false;
}
bool AutorizationController::logOut(std::string username) {
    std::cout << "Uruchamiamy funkcje logOut z param " << username << std::endl;
    if(isLoggedIn(username)) {
        if( remove((pathToUserFolder_ + username).c_str()) != 0 )
            return false;
        return true;
    }
    std::cout << "niezalogowany " << username << std::endl;
    return false;
}