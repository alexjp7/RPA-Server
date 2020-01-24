#include "ConnectionMessage.h"
#include <iostream>

namespace RPA
{
    //Used for Outgoing message construction
    ConnectionMessage::ConnectionMessage(const int& _stateId, const int& _gameId, const int& _clientId, const std::string& _name)
    :Message(_stateId, _gameId, _clientId)
    {
        this->stateId = _stateId;
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->name = _name;
        this->serialize();
    }

    //Used for incoming messages default construction
    ConnectionMessage::ConnectionMessage(const std::string& jsonString)
    :Message(jsonString)
    {
        this->deserialize(jsonString);
    }

    void ConnectionMessage::deserialize(const std::string& jsonString)
    {
        json message = json::parse(jsonString);
        this->stateId = message["state_id"];
        this->gameId = message["game_id"];
        this->clientId = message["client_id"];
        this->name = message["name"];
    }

    void ConnectionMessage::serialize()
    {
        json message;
        message["state_id"] = this->stateId;
        message["game_id"] = this->gameId;
        message["client_id"] = this->clientId;
        message["name"] = this->name; 
        this->message = message.dump();
    }
    
    std::string ConnectionMessage::getClientName() {return this->name;}
    std::string ConnectionMessage::getMessage() { return this->message;}
}
