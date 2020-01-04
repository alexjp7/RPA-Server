#include "Player.h"
#include "Game.h"
#include <iostream>

namespace RPA
{
    Game::Game()
    {
        //Do nothing
    }

    //Add the intial player to player list
    Game::Game(const unsigned int& playerId, const std::string& name)
    {   //the game id is the same as the client Id who created the game
        this->gameId = playerId;
        this->addPlayer(playerId, name);
    }

    void Game::addPlayer(const unsigned int& playerId, const std::string& name)
    {
        this->players.push_back(new Player(playerId, name));
    }

    unsigned int Game::getId() const
    {
        return this->gameId;
    }

    //Remove client from game - typically called upon dissconection
    void Game::removePlayer(const unsigned int& playerId)
    {
        int playerBeingRemoved = -1;
        for (unsigned int i = 0; i < this->players.size(); i++)
        {
            if(this->players[i]->getClientId() == playerId)
            {
                playerBeingRemoved = i;
            }
        }

        this->players.erase(this->players.begin() + playerBeingRemoved);
    }
    //Returns player specified by their client Id
    const Player* Game::getPlayer(const unsigned int& playerId) const
    {
        for(unsigned int i = 0; i < this->players.size(); i++)
        {
            RPA::Player* p = this->players[i];
            if(p->getClientId() == playerId)
            {
                return p;
            }
        }
        return NULL;
    }

    //returns all players conencted to a game
    const std::vector<Player*> Game::getAllPlayers() const
    {
        return this->players;
    }
    

    Game::~Game()
    {
        (this->players).clear();
    }

}



