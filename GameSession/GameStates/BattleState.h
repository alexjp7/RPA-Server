#pragma once
#include <memory>

#include "GameState.h"

#include "../../Enums/MessageType.cpp"
#include "../../Enums/GameState.cpp"
//Message Types
#include "../../Server/MessageTypes/BattleStateMessage.h"

namespace RPA
{
    class BattleState: public GameState
    {
        public:
            BattleState();
            ~BattleState();
            void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&players, const std::string& message);
            std::string getClientMessage();
            
    };
}