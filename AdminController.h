//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <string>
#include <fstream>
#include <dirent.h>
#include "AutorizationController.h"

class AdminController
{
    const std::string usersFile="../users.db";
    const std::string activeUsersPath="../loggedUsers/*";
public:
    std::string serverReset();
    std::string showActivePlayers();
    std::string showAllUsers();
    std::string kickUser(std::string user);
    std::string deleteUser(std::string user);
};


#endif // ADMIN_CONTROLLER_H
