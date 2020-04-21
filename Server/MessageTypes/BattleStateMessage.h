#pragma once
#include "Message.h"

namespace RPA
{
    class BattleStateMessage: public Message
    {
        private:
            RPA::BattleStateInstruction instructionType;

        private:
            void deserialize(const std::string&); //Override from message
            void serialize(const std::vector<std::unique_ptr<RPA::Player> >&);

        public: 
            BattleStateMessage();
            BattleStateMessage(const std::string&);
            ~BattleStateMessage() {}
            RPA::BattleStateInstruction getInstruction();
    };

}
