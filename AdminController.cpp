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

std::string AdminController::request(std::string command)
{
    std::string result;

    std::stringstream stream(command);
    std::vector<std::string> commandVector;

    while(stream.good() ){
        std::string tmp;
        getline(stream, tmp, ' ');
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
    else if(commandVector[0]=="delete")
    {
        if(commandVector.size() < 2)
            result = "No argument found. Command should have form: delete [username]";
        else
            result = deleteUser(commandVector[1]);
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
        result = "Commands:\r\n kick [username] \r\n delete [username] \r\n restart \r\n show all \r\n show active");
    else (result = "Unknown command : " + command);

    return result;
}
