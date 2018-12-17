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
            if(userInfo.length() == username.length() + passw.length() + 1) {
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
            bool successInCreating = false;
            std::cout << "tworzymy plik gracza" << std::endl;
            std::ofstream userFile(pathToUserFolder_ + username);
            if (userFile) successInCreating = true;
            userFile.close();
            return successInCreating;
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

//untested!!!
bool AutorizationController::registerUser(std::string username, std::string passw) {
    // check if this username exists
    std::ifstream dbFile;
    dbFile.open(pathToUserDatabase_);
    bool userFound = false;
    std::cout << "username = " << username << " ;password = " << passw << std::endl;
    if (dbFile) {
        std::string userInfo;
        while (!userFound && getline (dbFile,userInfo) ) {
           // std::cout << "szukamy... " << userInfo.length() <<" " << username.length() << " " << passw.length() << std::endl;
           //     std::cout << "kandydat... " << std::endl;
                std::size_t posUsername = userInfo.find(username);
                std::size_t posSeparator = userInfo.find(":");
                std::cout << "posUsername = " << posUsername << " ;posSeparator " << posSeparator
                << " len = " << username.length() << std::endl;
                if(posUsername == 0 && username.length() == posSeparator) {
                    userFound = true;
                }
            }
    }
    dbFile.close();
    if (userFound) {
        std::cout << "Taki użytkownik już istnieje" << std::endl;
        return false;
    }

    //content preparing
    std::string content = username + ":" + passw + "\n";
    std::cout << "Dajemy content: " << content; 

    //MUTEX?
    std::ofstream dbFile_app;
    dbFile_app.open(pathToUserDatabase_, std::ios_base::app);
    if (dbFile_app) {
        dbFile_app << content;
        return true;
    }
    
    dbFile_app.close();

    return false;
}