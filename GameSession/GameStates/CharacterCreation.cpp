#include "CharacterCreation.h"

namespace RPA
{
    CharacterCreation::CharacterCreation() : GameState()
    {

    }
    
   std::string CharacterCreation::getClientMessage()
    {
        return this->returnedClientMessage;
    }

    void CharacterCreation::processInstruction(const std::vector<std::unique_ptr<RPA::Player> >& players, const std::vector<std::string>& instruction)
    {
        switch (instruction[3][0])
        {
            case 'c': //class change
                // 4 = =
                this->returnedClientMessage = "c," + instruction[4];
                break;
            
            default:
                break;
        }
    } 

    CharacterCreation::~CharacterCreation()
    {

    }
    
}
