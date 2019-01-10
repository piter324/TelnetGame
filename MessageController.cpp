#include "MessageController.h"

std::string MessageController::help()
{
    std::string result = "Available commands:\r\nsend [receiver][message]\r\ndelete [message id]\r\nshow all\r\nshow [message id]\r\nback\r\n";
    return result;
}

std::string MessageController::deleteMessage(std::vector<std::string> commandVector)
{
    std::string result;
    MessageUsers messageUsers;

    if(commandVector.size() < 3)
        return "Command delete requires id of message you want to delete.\r\n";

    std::string username = commandVector[0];
    int id = std::atoi(commandVector[2].c_str());
    if(id <= 0)
        return "Message id should be a number greater than 0.\r\n";

    int deleteResult = messageUsers.telnetDeleteMessage(username, id);

    if(deleteResult == 0)
        result = "Message deleted!\r\n";
    else
        result = "Message with id " + std::to_string(id) + " cannot be deleted.\r\n";


    return result;
}

std::string MessageController::openSingleMessage(std::vector<std::string> commandVector)
{
    MessageUsers messageUsers;

    if(commandVector.size() < 3)
        return "Command delete requires id of message you want to delete.\r\n";

    std::string username = commandVector[0];
    int id = std::atoi(commandVector[2].c_str());
    if(id <= 0)
        return "Message id should be a number greater than 0.\r\n";

    return messageUsers.telnetOpenSingleMessage(username, id);
}

std::string MessageController::send(std::string command)
{
    std::stringstream stream(command);
    std::vector<std::string> commandVector;

        while(stream.good() )
        {
            std::string tmp;
            std::getline(stream, tmp, ' ');
            commandVector.push_back(tmp);
        }

    int sendResult;
    MessageUsers messageUsers;
    if(commandVector.size() < 4)
        return "To send message type: send [receiver] [message].\r\n";

    std::string username = commandVector[0];
    std::string receiver = commandVector[2];
    std::string message = "";
    for(int i = 3; i < commandVector.size(); ++i)
        message+= commandVector[i] + " ";

    sendResult = messageUsers.telnetSendMessage(username, receiver, message);

    std::string result;
    switch (sendResult)
    {
        case 0:     result = "Message to " + receiver + " sent successfully.\r\n";
                    break;
        case -2:    result = "User " + receiver + " does not exist.\r\n";
                    break;
        case -1:    result = "No access to " + receiver + " mailbox. Try again later or contact administrator.\r\n";
                    break;
    }

    return result;
}

std::string MessageController::showAllMessages(std::vector<std::string> commandVector)
{
    MessageUsers messageUsers;
    std::string username = commandVector[0];

    return messageUsers.telnetOpenMessages(username);
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
        //commandVector[1] is a command word
        //rest of vector are parameters
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

int MessageController::getNumberOfMessagess(std::string username){
    MessageUsers messageUsers;
    return messageUsers.telnetNumberOfMessages(username);
}