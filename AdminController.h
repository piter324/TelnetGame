//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <string>
#include <fstream>
#include <istream>
#include <dirent.h>
#include <sstream>
#include <vector>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "AutorizationController.h"

class AdminController
{
    const std::string usersFile="users.db";
    const std::string activeUsersPathForRestart="loggedUsers/*";
    const std::string activeUsersPathToShowList="loggedUsers/";
    sem_t* semUsers;
    std::string serverReset();
    std::string showActivePlayers();
    std::string showAllUsers();
    std::string kickUser(std::string user);
    std::string deleteUser(std::string user);
public:
    std::string request(std::string command);
    AdminController()
    {
        semUsers = sem_open("semUsers",  O_CREAT, 0644, 1);
    }
};


#endif // ADMIN_CONTROLLER_H
