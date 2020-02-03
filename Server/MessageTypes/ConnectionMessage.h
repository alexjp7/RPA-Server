#pragma once
#include "Message.h"

namespace RPA
{
    class ConnectionMessage: public Message
    {
        private:
            std::string name;
            RPA::ConnectionInstruction instructionType;

        private:
            void deserialize(const std::string&); //Override from message
            void serialize(const std::vector<std::unique_ptr<RPA::Player> >&);
      
        public: 
            ConnectionMessage();
            ~ConnectionMessage() {}
            //For Connection messages
            ConnectionMessage(const std::string&);
            ConnectionMessage(const int&, const int&, const RPA::State&, const std::string&, 
                              const std::vector<std::unique_ptr<RPA::Player> >&, const RPA::ConnectionInstruction& = ConnectionInstruction::NEW_GAME);
            //For disconnetion messages
            ConnectionMessage(const int&, const RPA::State&, const RPA::ConnectionInstruction& instructionType = ConnectionInstruction::NEW_GAME);

            std::string getClientName();
    };
}