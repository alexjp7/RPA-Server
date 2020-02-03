#include "ConnectionMessage.h"
#include <iostream>

namespace RPA
{   
    ConnectionMessage::ConnectionMessage()
    {
        this->name = "";
    }

    //Used for Outgoing message construction
    ConnectionMessage::ConnectionMessage(const int& _gameId, const int& _clientId, const RPA::State& _stateId,
    const std::string& _name, const std::vector<std::unique_ptr<RPA::Player> >& players, const RPA::ConnectionInstruction& instructionType)
    :Message(_gameId, _clientId)
    {
        this->stateId = _stateId;
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->name = _name;
        this->instructionType = instructionType;
        this->serialize(players);
    }

    //Used for dissconection message serialization
    ConnectionMessage::ConnectionMessage(const int& _clientId, const RPA::State& _stateId ,const RPA::ConnectionInstruction& instructionType)
    {
        //Game Message
        json gameMessage;
        gameMessage["state_id"] = _stateId;
        gameMessage["client_id"] = _clientId;
        gameMessage["instructionType"] = CreationInstruction::DISCONNECTION;
        this->gameMessage = gameMessage.dump();
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
        //Client Message
        json clientMessage;
        clientMessage["state_id"] = this->stateId;
        clientMessage["game_id"] = this->gameId;
        clientMessage["players"] = players;
        this->clientMessage = clientMessage.dump();
        
        if(this->instructionType == ConnectionInstruction::JOIN_GAME)
        {
            //Game Message
            json gameMessage;
            gameMessage["state_id"] = this->stateId;
            gameMessage["client_id"] = this->clientId;
            gameMessage["name"] = this->name;
            gameMessage["instructionType"] = CreationInstruction::CONNECTION;
            this->gameMessage = gameMessage.dump();
        }
    }   
    
    std::string ConnectionMessage::getClientName() {return this->name;}
}
