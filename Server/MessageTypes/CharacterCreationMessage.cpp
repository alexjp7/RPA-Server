#include "CharacterCreationMessage.h"
#include <iostream>

namespace RPA
{   
    CharacterCreationMessage::CharacterCreationMessage()
    {
        
    }
    //Used for Outgoing message construction
    CharacterCreationMessage::CharacterCreationMessage(const int& _gameId, const int& _clientId, 
                                                       const std::vector<std::unique_ptr<RPA::Player> >& players, const RPA::CreationInstruction& instructionType)
    :Message(_gameId, _clientId)
    {
        this->stateId = RPA::State::CHARACTER_CREATION;
        this->gameId = _gameId;
        this->clientId = _clientId;
        this->instructionType = instructionType;
        this->serialize(players);
    }

    void CharacterCreationMessage::serialize(const std::vector<std::unique_ptr<RPA::Player> >&)
    {
        //Server does not mutate game state data during character creation
    }

    //Used for incoming messages default construction
    CharacterCreationMessage::CharacterCreationMessage(const std::string& jsonString)
    :Message(jsonString)
    {
        this->deserialize(jsonString);
    }

    void CharacterCreationMessage::deserialize(const std::string& jsonString)
    {
        json message = json::parse(jsonString);
        this->stateId = message["state_id"];
        this->clientId = message["client_id"];
        this->instructionType =  message["instructionType"];

        if(this->instructionType == RPA::CreationInstruction::CLASS_CHANGE) 
        {
            this->playerAdventuringClass  = message["adventuringClass"];
        }
        else if(this->instructionType == RPA::CreationInstruction::READY_UP) 
        {
            this->playerReadyStatus = message["ready"];
        }
    }
    RPA::CreationInstruction CharacterCreationMessage::getInstruction(){return this->instructionType;}
    bool CharacterCreationMessage::getReadyStatus() {return this->playerReadyStatus;}
    unsigned int CharacterCreationMessage::getAventuringClass() {return this->playerAdventuringClass;}
}
