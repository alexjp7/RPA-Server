#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../../GameSession/Player.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace RPA
{

    class Message
    {
        protected:
            int stateId;
            int gameId;
            int clientId;
            std::string message;

        private:
            virtual void deserialize(const std::string&) = 0;
            virtual void serialize(const std::vector<std::unique_ptr<RPA::Player> >&) = 0;
            
        public:
            Message();
            Message(const std::string&);
            Message(const int& _stateId, const int& _gameId, const int& _clientId);
            virtual std::string getMessage() = 0;
            int getStateId() const;
            int getGameId() const;

    };
    
}
