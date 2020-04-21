#include "StateManager.h"
#include "../../Enums/GameState.cpp"
namespace RPA
{
    //Add character creation state to states vector upon construction
    StateManager::StateManager()
    {
        auto characterCreationState = std::make_unique<CharacterCreation>();
        states.push_back(std::move(characterCreationState));
    }

    void StateManager::addState(RPA::State newGameState, bool isRemovingPrevious)
    {
        if(isRemovingPrevious)
        {
            states.erase(states.end());
        }

        switch (newGameState)
        {
            case RPA::State::BATTLE_STATE:
                states.push_back(std::make_unique<RPA::BattleState>());
                break;
            default:
                break;
        }


    }

    std::unique_ptr<GameState> const& StateManager::getCurrentState() const 
    {
         return this->states.back(); 
    }

    StateManager::~StateManager() 
    {
        this->states.clear();
    }
} 
