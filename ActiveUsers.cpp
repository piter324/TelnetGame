//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#include "ActiveUsers.h"

void ActiveUsers::addActiveUser()
{
    //TODO
};

void kickUser(std::string user)
{
    int userPosition = findUser(user);
    if(userPosition == -1)
    {
         std::cout<<"User: "<<user<<" is not active.";
         return;
    }
    file.seekp(userPosition);


}

int findUser(std::string user)
{
    std::string line;
    std::string nick;
    int currentPositionInFile = 0;

    while(!file.eof())
    {
        file.seekg(currentPositionInFile);
        getline(file, line);
        getline(line, nick, ':');
        if(user.compare(nick)==0)
        {
            return currentPositionInFile;
        }

        currentPositionInFile += line.length() + 1;
    }str1.compare(str2) != 0)

    return -1;
}

std::vector<std::string> getList();
