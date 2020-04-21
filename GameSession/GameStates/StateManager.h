/*---------------------------------------------------------------
                        STATE=MANAGER
 ---------------------------------------------------------------*/
/***************************************************************
* Manages a vector of game states, through exposing
  access to vector of GameState instances which represent
  the current GameState stack of the client side game.
**************************************************************/
#pragma once

#include <memory>
#include <vector>
#include "GameState.h"
#include "CharacterCreation.h"
#include "BattleState.h"

namespace RPA
{   
    class StateManager
    {
        private:
            /***************************************************************
            * Collection of Gamestate objects that can be operated on.
            each GameState object's sub-type has enclosed functionality
            to perform mutations on player/game data aswell as
            indicating when states should be changed.
            **************************************************************/
            std::vector<std::unique_ptr<RPA::GameState> > states;

        public:
            StateManager();
            ~StateManager();

            /***************************************************************
            * Allows for new game states to be added, with the ability
              to flag whether the previous state is no longer needed

            @param newGameState: The next game state as request by client
            @param isRemovingPrevious: true - The previous state is removed
                                       false - The previous state is maintained 
            ********************************************lclaer
            c******************/
            void addState(RPA::State newGameState, bool isRemovingPrevious);

            /***************************************************************
            @return The current/active (last in vector) state in the
            GameState vector. 
            **************************************************************/
            std::unique_ptr<GameState> const& getCurrentState() const;
    };
    
} // namespace RPA

