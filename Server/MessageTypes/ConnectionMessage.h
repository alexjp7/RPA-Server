#pragma once

#include "Message.h"

namespace RPA
{
    class ConnectionMessage: public Message
    {
        private:
            std::string name;

        private:
            void deserialize(const std::string&); //Override from message
            void serialize();
            
        public: 
            ConnectionMessage(const std::string&);
            ConnectionMessage(const int& _stateId, const int& _gameId, const int& _clientId, const std::string& _name);
            std::string getClientName();
            std::string getMessage();
    };
}