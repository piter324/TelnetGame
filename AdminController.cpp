//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#include "AdminController.h"

std::string AdminController::serverReset()
{
    system("exec rm -r " + activeUsersPath);
    return("Server restarted.");
}

std::string AdminController::showActivePlayers()
{
    std::string line;
    std::string nick;
    std::string result;

    AutorizationController autorizationController;
    DIR *dir;
    struct dirent *ent;

    if(dir.opendir(activeUsersPath) != NULL)
    {
        while(ent = readdir(dir) != NULL)
        {
            std::string fileName = ent->d_name;
            std::string nick;
            getline(fileName, nick, '.');
            result+=nick + "\r\n";
        }
        closedir(dir);
        return result;
    }
    return("Cannot enter location \"" + activeUsersPath + "\".");
}

std::string AdminController::showAllUsers()
{
    std::fstream file;
    file.open(usersFile, std::ios::in);
    std::string line;
    std::string nick;
    std::string result;

    if(file.good())
    {
        while(!file.eof())
        {
            getline(file, line);
            getline(line, nick, ':');
            result += nick;
            if(!file.eof())
                result +="\r\n";
        }
        return result;
    }
    else std::cout<<"Cannot open file \""<<usersFile<<"\"."<<std::endl;
}

//kick user == logout user
std::string AdminController::kickUser(std::string user)
{
    AutorizationController autorizationController;
    if(autorizationController.logOut(user))
        return ("User \"" + user + "\" was kicked from the server.");
    return ("User \"" + user + "\" is not active or does not exist.");
}

std::string AdminController::deleteUser(std::string user)
{
    std::fstream file;
    file.open(usersFile, std::ios::in);
    std::string line;
    std::string nick;
    if(file.good())
    {
        while(!file.eof())
        {
            getline(file, line);
            getline(line, nick, ':');
            if(nick==user)
            {
                // delete user
                return  ("User \""+ user + "\"deleted.");
            }
        }
        return ("User \""+ user + "\"does not exist.");
    }
    else return("Cannot open file \"" + usersFile + "\".");

}
