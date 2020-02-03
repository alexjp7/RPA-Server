#pragma once
#include "Message.h"

namespace RPA
{
    class CharacterCreationMessage: public Message
    {
        private:
            RPA::CreationInstruction instructionType;
            unsigned int playerAdventuringClass;
            bool playerReadyStatus;

        private:
            void deserialize(const std::string&); //Override from message
            void serialize(const std::vector<std::unique_ptr<RPA::Player> >&);

        public: 
            CharacterCreationMessage();
            ~CharacterCreationMessage() {}
            CharacterCreationMessage(const std::string&);
            CharacterCreationMessage(const int&, const int&, 
                                     const std::vector<std::unique_ptr<RPA::Player> >&,  const RPA::CreationInstruction&);
            RPA::CreationInstruction getInstruction();
            bool getReadyStatus();
            unsigned int getAventuringClass();
    };

}