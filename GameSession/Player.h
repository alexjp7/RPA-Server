#pragma once
#include <string>
namespace RPA
{
    //enum PlayerClass {WARRIOR = 'w', WIZARD = 'z', ROGUE = 'r', CLERIC = 'c'};
    class Player
    {
        private:
            //PlayerClass playerClass;
            std::string name;
            unsigned int goldCoins;
            unsigned int clientId;
            unsigned int partyId;

        public:
            Player(const unsigned int&, const std::string&, const unsigned int&);
            ~Player();
            
            char getClass() const;
            std::string getName() const;
            unsigned int getClientId() const;
            unsigned int getPartyId() const;

            void setClass(const char&);
            void setName(const std::string&);
    };
}