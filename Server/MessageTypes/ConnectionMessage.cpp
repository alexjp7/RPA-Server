#include "ConnectionMessage.h"
#include <iostream>

namespace RPA
{   
    ConnectionMessage::ConnectionMessage()
    {
        this->name = "";
    }

    //Used for Outgoing message construction
    ConnectionMessage::ConnectionMessage(const int& _stateId, const int& _gameId, const int& _clientId, 
    const std::string& _name, const std::vector<std::unique_ptr<RPA::Player> >& players)
    :Message(_stateId, _gameId, _clientId)
    {
        this->stateId = _stateId;
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->name = _name;
        this->serialize(players);
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

    void ConnectionMessage::serialize(const std::vector<std::unique_ptr<RPA::Player> >& players)
    {
        json message;
        message["state_id"] = this->stateId;
        message["game_id"] = this->gameId;
        message["players"] = players;
        this->message = message.dump();
    }   
    
    std::string ConnectionMessage::getClientName() {return this->name;}
    std::string ConnectionMessage::getMessage() { return this->message;}



}
