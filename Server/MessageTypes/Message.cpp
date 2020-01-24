#include "Message.h"

namespace RPA
{
    Message::Message(const std::string& message)
    {
        this->stateId = -1;
        this->gameId = -1;
        this->clientId = -1;
        this->message = "";

    }
    
    Message:: Message(const int& _stateId, const int& _gameId, const int& _clientId)
    {
        this->stateId = _stateId;
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->message = "";
    }

    int Message::getStateId() const {return this->stateId;}
    int Message::getGameId()  const {return this->gameId;}
}