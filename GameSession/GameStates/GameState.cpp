#include "GameState.h"

namespace RPA
{
    GameState::GameState()
    {
          this->originMessage = false;
          this->returnedClientMessage = "";
    }

    std::unique_ptr<Player> const&  GameState::getPlayer(const std::vector<std::unique_ptr<RPA::Player> >& players, const unsigned int& playerId)
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
    
    /******************************************************
    @return Whether client is moving from a game state to 
    another, and does not need previous one
    *******************************************************/
    bool GameState::isRemovingPrevious() const
    {
        return this->isRemoving; 
    }

    /******************************************************
    @return Whether the has recieved an instruction to 
    updadte/change the current game state.
    *******************************************************/
    bool GameState::hasRecievedStateChanged() const
    {
        return this->hasNextState; 
    }

    bool GameState::hasOriginMessage() const
    { 
        return this->originMessage;
    }

    RPA::State GameState::getNextState() const {return this->nextState;}
    RPA::State GameState::getStateId()  const {return this->stateId;}

}

