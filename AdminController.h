//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <string>

class AdminController
{
    void serverReset();
    void showActivePlayers();
    void showAllUsers();
    void kickUser(std::string user);
    void deleteUser(std::string user);
};


#endif // ADMIN_CONTROLLER_H
