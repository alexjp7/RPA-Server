#pragma once
#include <string>
namespace RPA
{
    enum ADVENTURING_CLASS {WARRIOR = 0, WIZARD, ROGUE, CLERIC};
    class Player
    {
        private:
            std::string name;
            unsigned int goldCoins;
            unsigned int playerClass;
            unsigned int clientId;
            unsigned int partyId;

        public:
            Player(const unsigned int&, const std::string&, const unsigned int&);
            ~Player();
            
            std::string getName() const;
            unsigned int getClass() const;
            unsigned int getClientId() const;
            unsigned int getPartyId() const;

            void setClass(const unsigned int&);
            void setName(const std::string&);
    };
}