#pragma once
#include <vector>
#include <utility>
#include <memory>

#include "Player.h"
#include "GameStates/StateManager.h"

namespace RPA
{
    class Game
    {
        public:
            static const unsigned int PARTY_LIMIT = 4;
        private:
            unsigned int gameId;
            std::vector<std::unique_ptr<Player> > players;
            std::unique_ptr<StateManager> stateManager;


        private:
         void resize(unsigned int&);
         void updatePlayerData();

        public:
            Game();
            Game(const unsigned int&, const std::string&); //Id of the client who created game
            ~Game();
            bool addPlayer(const unsigned int&,  const std::string&);
            void removePlayer(const unsigned int&);

            std::string processInstruction(const std::vector<std::string>&);

            unsigned int getId() const;
            unsigned int getPartySize() const;
            std::unique_ptr<Player> const& getPlayer(const unsigned int&) const;
            std::vector<std::unique_ptr<Player> >  const& getAllPlayers() const;

            

    };
}