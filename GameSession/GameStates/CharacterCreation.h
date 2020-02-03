#pragma once
#include <memory>

#include "GameState.h"

#include "../../Enums/MessageType.cpp"
#include "../../Enums/GameState.cpp"
//Message Types
#include "../../Server/MessageTypes/CharacterCreationMessage.h"

namespace RPA
{
    class CharacterCreation: public GameState
    {
        public:
            CharacterCreation();
            ~CharacterCreation();
            void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&players, const std::string& message);
            std::string getClientMessage();
            
    };
}