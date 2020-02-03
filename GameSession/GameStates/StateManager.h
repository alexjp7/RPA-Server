#pragma once
#include <memory>
#include <vector>
#include "GameState.h"
#include "CharacterCreation.h"


namespace RPA
{   
    class StateManager
    {
        private:
            std::vector<std::unique_ptr<RPA::GameState> > states;
            void addState();
            void removeState();

        public:
            StateManager();
            ~StateManager();

            std::unique_ptr<GameState> const& getCurrentState() const;
    };
    
} // namespace RPA

