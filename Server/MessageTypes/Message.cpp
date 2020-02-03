#include "Message.h"

namespace RPA
{
    Message::Message()
    {
        this->gameId = -1;
        this->clientId = -1;
        this->clientMessage = "";
        this->gameMessage = "";
    }

    Message::Message(const std::string& message)
    {
        this->gameId = -1;
        this->clientId = -1;
        this->clientMessage = "";
        this->gameMessage = "";
    }
    
    Message:: Message(const int& _gameId, const int& _clientId)
    {
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->clientMessage = "";
        this->gameMessage = "";
    }

    std::string& Message::getClientMessage() { return this->clientMessage;}
    std::string& Message::getGameMessage() { return this->gameMessage;}
    RPA::State Message::getStateId() const {return this->stateId;}
    int Message::getGameId()  const {return this->gameId;}
    int Message::getClientId()  const {return this->clientId;}
}