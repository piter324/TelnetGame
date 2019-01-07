/*
 * @author Karolina Bilewicz
 * @version 0.1
 * 
*/


#ifndef TELNETGAME_AUTORIZATION_H
#define TELNETGAME_AUTORIZATION_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ios>


class AutorizationController {
    public:
        AutorizationController(std::string pathToUserDatabase = std::string("users.db"),
         std::string pathToUserFolder = std::string("loggedUsers/"), std::string pathToMessageFolder = std::string("messagesUsers/") ) {
            pathToUserDatabase_ = pathToUserDatabase;
            pathToUserFolder_ = pathToUserFolder;
            pathToMessageFolder_ = pathToMessageFolder;
        }

        bool isLoggedIn(std::string username);
        bool logIn(std::string username, std::string passw);
        bool logOut(std::string username);
        bool registerUser(std::string username, std::string passw);
    private:
        std::string pathToUserDatabase_;
        std::string pathToUserFolder_;
        std::string pathToMessageFolder_;
};


#endif // TELNETGAME_AUTORIZATION_H