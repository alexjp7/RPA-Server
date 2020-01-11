#include "StateManager.h"
#include "GameState.h"
#include "CharacterCreation.h"

namespace RPA
{
    //Add character creation state to states vector upon construction
    StateManager::StateManager()
    {
        auto characterCreationState = std::make_unique<CharacterCreation>();
        states.push_back(std::move(characterCreationState));

    }

     std::unique_ptr<GameState> const& StateManager::getCurrentState() const
    {
        return this->states.back();
    }

    void StateManager::processStateChange(const std::vector<std::string> & instruction)
    {
/*         switch (instruction[3][0])
        {
            case 'c':

                break;
            
            default:
                break;
        } */
    }

    StateManager::~StateManager()
    {
        this->states.clear();
    }

    
} // namespace RPA
