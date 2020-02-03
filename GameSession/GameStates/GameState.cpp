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

    bool GameState::hasOriginMessage() const{ return this->originMessage;}
}

