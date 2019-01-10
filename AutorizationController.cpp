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
    std::size_t posSep = username.find(":");
    if (posSep != std::string::npos)
        return false; //innapropriate username
    std::cout << "user ok\n";
    posSep = passw.find(":");
        if (posSep != std::string::npos)
        return false; //innapropriate password
    std::cout << "passw ok \n";
    sem_wait(semUsers);
    std::ifstream dbFile(pathToUserDatabase_);
    bool userFound = false;
    //spr username i hasła
    if (dbFile) {
        std::cout<<"File open\n";
        std::string userInfo;
        while (!userFound && getline(dbFile,userInfo) ) {
            // std::cout << "szukamy... " << userInfo.length() <<" " << username.length() << " " << passw.length() << std::endl;
            std::cout << "kandydat... " << userInfo << std::endl;
            std::size_t posUsername = userInfo.find(username);
            posSep = userInfo.find(":");
            std::size_t posPasswd = userInfo.find(passw, posSep);
            // std::cout << "szukamy2... " << posUsername <<" " << username.length() << " " << posPasswd << std::endl;
            if(posUsername != std::string::npos && posPasswd != std::string::npos) {
                // std::cout << "szukamy3... " << posUsername <<" " << username.length() << " " << posPasswd << std::endl;
                std::string password = userInfo.substr(posPasswd);
                if(posUsername == 0 && posPasswd == username.length() + 1 && posPasswd-1 == username.length())
                    if(password.compare(passw) == 0 || (int(password[password.length() - 1]) == 13 && password.length() == passw.length() + 1) )
                    userFound = true;
            }
        }
        dbFile.close();
        // ok login + hasło
        if(userFound) {
            if(isLoggedIn(username))
            {
                sem_post(semUsers);
                return false;
            }
            bool successInCreating = false;
            std::cout << "tworzymy plik gracza" << std::endl;
            std::ofstream userFile(pathToUserFolder_ + username);
            if (userFile) successInCreating = true;
            userFile.close();
            sem_post(semUsers);
            return successInCreating;
        }
        //todo sprawdz czy user juz zalogowany
    }
    dbFile.close();
    std::cout << "blad w otwieraniu pliku" << std::endl;
    sem_post(semUsers);
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
    std::size_t posSep = username.find(":");
    if (posSep != std::string::npos)
        return false; //innapropriate username
    posSep = passw.find(":");
    if (posSep != std::string::npos)
        return false; //innapropriate password
    // check if this username exists
    std::ifstream dbFile;
    sem_wait(semUsers);
    dbFile.open(pathToUserDatabase_);
    bool userFound = false;
    bool anyUser = false;
    std::cout << "username = " << username << " ;password " << passw << std::endl;
    if (dbFile ) {
        std::string userInfo;
        while (!userFound && getline (dbFile,userInfo) ) {
           // std::cout << "szukamy... " << userInfo.length() <<" " << username.length() << " " << passw.length() << std::endl;
           //     std::cout << "kandydat... " << std::endl;
                anyUser = true;
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
    sem_post(semUsers);
    if (userFound) {
        std::cout << "Taki użytkownik już istnieje" << std::endl;
        return false;
    }

    //content preparing
    std::string content;
    if(anyUser)
        content = "\n" + username + ":" + passw;
    else
        content =  username + ":" + passw;
    std::cout << "Dajemy content: " << content;



    //MUTEX?
    sem_wait(semUsers);
    std::ofstream dbFile_app;
    std::ofstream msgFile(pathToMessageFolder_ + username);
    dbFile_app.open(pathToUserDatabase_, std::ios_base::app);
    //msgFile_app.open(pathToMessageFolder_ + username, std::ios::out);
    if (dbFile_app && msgFile) {
        dbFile_app << content;
        dbFile_app.clear();
        dbFile_app.seekp(0,  dbFile_app.beg);
        return true;
    }

    dbFile_app.close();
    msgFile.close();
    sem_post(semUsers);

    return false;
}
