#pragma once
#include <string>
namespace RPA
{
    class Player
    {
        private:
            char p_class;
            std::string name;
            unsigned int goldCoins;
            unsigned int clientId;

        public:
            Player(const unsigned int&, const std::string&);
            ~Player();
            
            char getClass() const;
            std::string getName() const;
            const unsigned int getClientId() const;

            void setClass(const char&);
            void setName(const std::string&);
    };
}