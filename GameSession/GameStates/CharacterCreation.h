#pragma once
#include <memory>

#include "GameState.h"
namespace RPA
{
    class CharacterCreation: public GameState
    {
        public:
            CharacterCreation();
            ~CharacterCreation();
            void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&, const std::vector<std::string>&);
            std::string getClientMessage();
            std::unique_ptr<Player> const& getPlayer(const std::vector<std::unique_ptr<RPA::Player> >& players, const unsigned int& playerId) const;
    };
}