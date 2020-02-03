#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../Player.h"
#include "../../Enums/GameState.cpp"
#include "../../Enums/MessageType.cpp"

namespace RPA
{
    class GameState
    {
        protected:
            std::string returnedClientMessage;
            RPA::State stateId;
            bool originMessage;
            std::unique_ptr<Player> const&  getPlayer(const std::vector<std::unique_ptr<RPA::Player> >& players, const unsigned int& playerId);
        public:
            GameState();
            virtual ~GameState(){}
            virtual std::string getClientMessage() = 0;
            virtual void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&players, const std::string& message) = 0; 
            RPA::State getStateId() {return this->stateId;}
            bool hasOriginMessage() const;


    };
}
