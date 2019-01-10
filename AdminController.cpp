//
// Created by Bartlomiej Jurek on 28/11/2018.
//

#include "AdminController.h"

std::string AdminController::serverReset()
{
    std::string command = "exec rm -r " + activeUsersPathForRestart;
    system(command.c_str());
    return("Server restarted.");
}

std::string AdminController::showActivePlayers()
{
    std::string line;
    std::string nick;
    std::string result;
    int activeUsersAmount = 0;

    AutorizationController autorizationController;
    DIR *dir;
    struct dirent *ent;
    dir = opendir(activeUsersPathToShowList.c_str());
    if(dir != nullptr)
    {
        ent = readdir(dir);
        while(ent != nullptr)
        {
            std::string fileName = ent->d_name;
            std::string nick;
            std::stringstream stream(fileName);
            std::getline(stream, nick, '.');
            result+=nick + "\n";
            ++activeUsersAmount;
            ent = readdir(dir);
        }
        closedir(dir);
        if(!activeUsersAmount)
            result = "No active players.";
        return result;
    }
    return("Cannot enter location \"" + activeUsersPathToShowList + "\".");
}

std::string AdminController::showAllUsers()
{
    std::fstream file;
    sem_wait(semUsers);
    file.open(usersFile, std::ios::in);
    std::string line;
    std::string nick;
    std::string result;
    int usersAmount = 0;

    if(file.good())
    {
        while(!file.eof())
        {
            std::getline(file, line);
            std::stringstream stream(line);
            std::getline(stream, nick, ':');
            result += nick + "\n";
            ++usersAmount;
        }
        if(!usersAmount)
            result = "No players found.";
        file.close();
        sem_post(semUsers);
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

/*
std::string AdminController::deleteUser(std::string user)
{
    std::fstream file;
    sem_wait(semUsers);
    file.open(usersFile, std::ios::in);
    std::string line;
    std::string nick;
    if(file.good())
    {
        while(!file.eof())
        {
            std::getline(file, line);
            std::stringstream stream(line);
            std::getline(stream, nick, ':');
            if(nick==user)
            {
                // delete user
                file.close();
                sem_post(semUsers);
                return  ("User \""+ user + "\"deleted.");
            }
        }
        file.close();
        sem_post(semUsers);
        return ("User \""+ user + "\"does not exist.");
    }
    sem_post(semUsers);
    return("Cannot open file \"" + usersFile + "\".");
}
*/

std::string AdminController::request(std::string command)
{
    std::string result;

    std::stringstream stream(command);
    std::vector<std::string> commandVector;

    while(stream.good() ){
        std::string tmp;
        std::getline(stream, tmp, ' ');
        commandVector.push_back(tmp);
        }

    if(commandVector.size() < 1)
        result = "No command found. \r\n Possible commands:\r\n kick [username] \r\n delete [username] \r\n restart \r\n show all \r\n show active \r\n ";

    if(commandVector[0]=="kick")
    {
        if(commandVector.size() < 2)
            result = "No argument found. Command should have form: kick [username]";
        else
            result = kickUser(commandVector[1]);
    }
    else if(commandVector[0]=="restart")
    {
        result = serverReset();
    }
    else if(commandVector[0]=="show")
    {
        if(commandVector.size() < 2)
            result = "No argument found. Command should have form: \"show all\" or \"show active\" ";
        else if(commandVector[1]=="all")
            result = showAllUsers();
        else if(commandVector[1]=="active")
            result = showActivePlayers();
        else result = "Argument " + commandVector[1] + "is not valid.";
    }
    else if(commandVector[0]=="help")
        result = "Commands:\r\n kick [username] \r\n restart \r\n show all \r\n show active";
    else result = "Unknown command : " + command;

    return result;
}
