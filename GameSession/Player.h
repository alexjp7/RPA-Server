#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace RPA
{
    enum ADVENTURING_CLASS {WARRIOR = 0, WIZARD, ROGUE, CLERIC};
    class Player
    {
        private:
            bool ready;
            std::string name;
            unsigned int goldCoins;
            int adventuringClass;
            unsigned int clientId;

        public:
            Player(const unsigned int&, const std::string&);
            ~Player();
            
            std::string getName() const;
            int getClass() const;
            bool isReady() const;
            unsigned int getClientId() const;

            void setClass(const unsigned int&);
            void setName(const std::string&);
            void toggleReady();
            std::string toDelimitedString();
            
    };
    void to_json(json& j, const std::unique_ptr<Player>& p);
}