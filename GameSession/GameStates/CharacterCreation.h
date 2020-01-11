#pragma once
#include "GameState.h"

#include <memory>
namespace RPA
{
    class CharacterCreation: public GameState
    {
        public:
            CharacterCreation();
            ~CharacterCreation();
            void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&, const std::vector<std::string>&);
            std::string getClientMessage();
    };
}