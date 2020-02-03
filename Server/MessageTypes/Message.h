#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include "../../GameSession/Player.h"
#include "../../Enums/MessageType.cpp"
#include "../../Enums/GameState.cpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace RPA
{
    class Message
    {
        protected:
            RPA::State stateId;
            int gameId;
            int clientId;
            std::string clientMessage;
            std::string gameMessage;

        private:
            virtual void deserialize(const std::string&) = 0;
            virtual void serialize(const std::vector<std::unique_ptr<RPA::Player> >&) = 0;
            
        public:
            Message();
            virtual ~Message() {}
            Message(const std::string&);
            Message(const int&, const int&);
            std::string& getClientMessage();
            std::string& getGameMessage();
            RPA::State getStateId() const;
            int getGameId() const;
            int getClientId() const;
    };
}
