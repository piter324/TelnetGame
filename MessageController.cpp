#include "MessageController.h"

std::string MessageController::help()
{
    std::string result = "Available commands:\r\nsend [receiver][message]\r\ndelete [message id]\r\nshow all\r\nshow [message id]\r\n";
    return result;
}

std::string MessageController::deleteMessage(std::vector<std::string> commandVector)
{
    std::string result;
    MessageUsers messageUsers;
    //result = messageUsers.telnetDeleteMessage(string login, int id);

    return result;
}

std::string MessageController::openSingleMessage(std::vector<std::string> commandVector)
{
    std::string result;
    MessageUsers messageUsers;
    //result = messageUsers.telnetOpenSingleMessage(string username, int id);

    return result;
}

std::string MessageController::send(std::string command)
{
    std::string result;
    MessageUsers messageUsers;
    //result = messageUsers.telnetSendMessage(string username, string receiver, string message);
    return result;
}

std::string MessageController::showAllMessages(std::vector<std::string> commandVector)
{
    std::string result;
    MessageUsers messageUsers;
    //result = messageUsers.telnetOpenMessages(string username);
    return result;
}

std::string MessageController::request(std::string command)
{
    std::string result;

    std::stringstream stream(command);
    std::vector<std::string> commandVector;

        while(stream.good() )
        {
            std::string tmp;
            std::getline(stream, tmp, ' ');
            commandVector.push_back(tmp);
        }

        //commandVector[0] is a username.
        if(commandVector[1]=="help")
        {
            result = help();
        }
        else if(commandVector[1]=="send")
        {
            result = send(command);
        }
        else if (commandVector[1]=="delete")
        {
            result = deleteMessage(commandVector);
        }
        else if(commandVector[1]=="show")
        {
            if(commandVector.size() < 3)
                result = "Command show require at least one more parameter.\r\n";
            else if(commandVector[2]=="all")
            {
                result = showAllMessages(commandVector);
            }
            else 
                result = openSingleMessage(commandVector);
        }
        else result = commandVector[1] + " is not a valid command. Write help for command list.\r\n";

    return result;
}