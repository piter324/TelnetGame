//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#ifndef ACTIVE_USERS_H
#define ACTIVE_USERS_H

#include <fstream>
#include <string>
#include <vector>
#include "AdminController.h"

class ActiveUsers
{
    fstream file;
    const std::string activeUsersPath = "../active_users.db";

    ActiveUsers()
    {
        file.open (activeUsersPath, std::fstream::in | std::fstream::out);
        if(!file.is_open())
            throw std::runtime_error("Could not open file");
    }

    ~ActiveUsers()
    {
        file.close();
    }

    friend class AdminController;
    void kickUser(std::string user);
    std::vector<std::string> getList();


public:
    static ActiveUsers& getInstance()
    {

        static ActiveUsers instance;
        return instance;
    }

    void addActiveUser();
};






#endif // ACTIVE_USERS_H
