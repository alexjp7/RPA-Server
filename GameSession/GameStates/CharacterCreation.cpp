#include "CharacterCreation.h"

namespace RPA
{
    CharacterCreation::CharacterCreation() : GameState()
    {

    }
    
   std::string CharacterCreation::getClientMessage()
    {
        // returnedClientMessage[0] = state instruction flag (indicates client that this instruction is relevant to a game state)
        // returnedClientMessage[1] = state instruction (particular state instruction)
        // returnedClientMessage[2] = player who instigated server request
        // returnedClientMessage[3-n] = state specific instruction
        return this->returnedClientMessage;
    }

    void CharacterCreation::processInstruction(const std::vector<std::unique_ptr<RPA::Player> >& players, const std::vector<std::string>& instruction)
    {
        switch (instruction[4][0])
        {
            case 'c':  //class change, client who changed, class changed too
                this->returnedClientMessage = "i,c," + instruction[3] + "," + instruction[5];
                //Update player class server side
                this->getPlayer(players,  (unsigned int)std::stoi(instruction[3]))->setClass(std::stoi(instruction[5]));
                break;
            
            default:
                break;
        }
    } 



    std::unique_ptr<Player> const& CharacterCreation::getPlayer(const std::vector<std::unique_ptr<RPA::Player> >& players, const unsigned int& playerId) const
    {
        for(auto& p: players)
        {
            if(p->getClientId() == playerId)
            {
                return p;
            }
        }
        return nullptr;
    }

    CharacterCreation::~CharacterCreation()
    {

    }
    
}
