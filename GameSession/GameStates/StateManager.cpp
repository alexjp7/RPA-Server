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

    std::unique_ptr<GameState> const& StateManager::getCurrentState() const { return this->states.back(); }
    StateManager::~StateManager() {this->states.clear();}
} // namespace RPA
