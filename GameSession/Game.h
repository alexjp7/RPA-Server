#pragma once
#include <vector>
#include <utility>
#include <memory>
#include <typeinfo> 
#include "Player.h"
#include "GameStates/StateManager.h"
#include "GameStates/GameState.h"

#include "../Enums/GameState.cpp"

namespace RPA
{
    class Game
    {
        public:
            static const unsigned int PARTY_LIMIT = 4;
        private:
            unsigned int gameId;
            unsigned int partyLeaderId;
            std::vector<std::unique_ptr<RPA::Player> > players;
            std::unique_ptr<RPA::StateManager> stateManager;

        private:
         void resize(unsigned int&);
         void updatePlayerData();
         void printPlayers() const;

        public:
            Game();
            Game(const unsigned int&, const std::string&); //Id of the client who created game
            ~Game();
            //Game Data
            const unsigned int getId() const;
            //Player getters/setters
            bool addPlayer(const unsigned int&,  const std::string&);
            void removePlayer(const unsigned int&);
            unsigned int getPartySize() const;
            std::unique_ptr<Player> const& getPlayer(const unsigned int&) const;
            std::vector<std::unique_ptr<Player> >  const& getAllPlayers() const;
            //State Operations
            std::string recieveInstruction(const std::string&);
            RPA::State getStateId() const;
            std::string getStateTypeId() const;
            bool hasOriginMessage() const;
    };
}