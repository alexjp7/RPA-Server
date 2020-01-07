#include <iostream>
#include "Player.h"
#include "Game.h"

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

    bool Game::addPlayer(const unsigned int& playerId, const std::string& name)
    {
        if(this->players.size() == PARTY_LIMIT) {return false;}
        auto player = std::make_unique<Player>(Player(playerId, name, this->players.size()));
        //this->players.push_back(std::move(player));
        return true;
    }

    unsigned int Game::getId() const
    {
        return this->gameId;
    }

    //Remove client from game - typically called upon dissconection
    void Game::removePlayer(const unsigned int& playerId)
    {
        unsigned int playerBeingRemoved = -1;
        for (unsigned int i = 0; i < this->players.size(); i++)
        {
            if(this->players[i]->getClientId() == playerId)
            {
                playerBeingRemoved = i;
            }
        }
        resize(playerBeingRemoved);
    }

    //Removes dissconected player while maintaining contiguous array
    void Game::resize(unsigned int& removedIndex)
    {
        if(removedIndex > 0) //TO-DO check index out of bounds
        {
            std::swap(this->players[removedIndex], this->players[this->players.size()]);
        }
        this->players.erase(this->players.end());
    }

    //Returns player specified by their client Id
    std::unique_ptr<Player> const& Game::getPlayer(const unsigned int& playerId) const
    {
        for(auto& p: this->players)
        {
            if(p->getClientId() == playerId)
            {
                return p;
            }
        }
        return nullptr;
    }

    //returns all players conencted to a game
    std::vector<std::unique_ptr<Player> > const& Game::getAllPlayers() const
    {
        return this->players;
    }

    unsigned int Game::getPartySize() const
    {
        return this->players.size();
    }
    

    Game::~Game()
    {
        (this->players).clear();
    }

}



