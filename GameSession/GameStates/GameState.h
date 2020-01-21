#pragma once
#include <vector>
#include <string>
#include <memory>

#include "../Player.h"

namespace RPA
{
    class GameState
    {
        protected:
            std::string returnedClientMessage;
        public:
            virtual ~GameState(){}
            virtual std::string getClientMessage() = 0;
            virtual void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&, const std::vector<std::string>&) = 0; 
    };
}
