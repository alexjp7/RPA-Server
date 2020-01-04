#pragma once
#include <vector>
#include <utility>

#include "Player.h"



namespace RPA
{
    class Game
    {
        private:
            unsigned int gameId;
            std::vector<Player*> players;
            //std::stack<GameState> gameStates;


        public:
            Game();
            Game(const unsigned int&, const std::string&); //Id of the client who created game
            ~Game();
            void addPlayer(const unsigned int&,  const std::string&);
            void removePlayer(const unsigned int&);

            unsigned int getId() const;
            const Player* getPlayer(const unsigned int&) const;
            const std::vector<Player*> getAllPlayers() const;
            //GameState getTopState() const; 

    };
}