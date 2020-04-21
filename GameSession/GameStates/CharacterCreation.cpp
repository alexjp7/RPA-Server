#include "CharacterCreation.h"

namespace RPA
{
    CharacterCreation::CharacterCreation() : GameState()
    {
        this->stateId = RPA::State::CHARACTER_CREATION;
        this->originMessage = false;
    }
    
    void CharacterCreation::processInstruction(const std::vector<std::unique_ptr<RPA::Player> >& players, const std::string& message)
    {
        RPA::CharacterCreationMessage clientMessage(message);
        
        switch (clientMessage.getInstruction())
        {
            case RPA::CreationInstruction::CLASS_CHANGE:
                this->getPlayer(players, clientMessage.getClientId())->setClass(clientMessage.getAventuringClass());
                break;

            case RPA::CreationInstruction::READY_UP:
                this->getPlayer(players, clientMessage.getClientId())->toggleReady();
                break;
                
            case RPA::CreationInstruction::GAME_START:
                this->hasNextState = true;
                this->isRemoving = true;
                this->nextState = RPA::State::BATTLE_STATE;
                break;

            default:
                break;
        }
        this->returnedClientMessage = message;
    } 

    std::string CharacterCreation::getClientMessage(){ return this->returnedClientMessage;}
    CharacterCreation::~CharacterCreation() {}
}
