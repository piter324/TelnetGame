#ifndef MESSAGE_CONTROLLER_H
#define MESSAGE_CONTROLLER_H

#include "Room.h"
#include "MessageUsers.h"
#include <string>
#include <sstream>
#include <vector>

class MessageController
{
    std::string showAllMessages(std::vector<std::string> commandVector);
    std::string deleteMessage(std::vector<std::string> commandVector);
    std::string openSingleMessage(std::vector<std::string> commandVector);
public:
    std::string request(std::string command);
    std::string send(std::string command);
    std::string help();
};

#endif //MESSAGE_CONTROLLER_H

// TODO
// obsluga w serwerze:
// - przy sprawdzaniu czy exit it sprawdzanie czy send jest slowem kluczowym
// - nowy stan przebywania w skrzynce pocztowej, do skrzynki wchodzimy slowem kluczowym mailbox
// - powrot ze stanu mailbox za pomoca exit powinno ponownie wyswietlic opis pomieszczenia w ktorym sie znajdujemy??
