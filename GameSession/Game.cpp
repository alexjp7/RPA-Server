#include <iostream>
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
        auto manager = std::make_unique<RPA::StateManager>(); //Pushes character creation state 
        this->stateManager = std::move(manager);
        this->addPlayer(playerId, name);
    }

    bool Game::addPlayer(const unsigned int& playerId, const std::string& name)
    {
        if(this->players.size() == PARTY_LIMIT) {return false;}
        auto player = std::make_unique<Player>(playerId, name);
        this->players.push_back(std::move(player));
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
        //Handle if no players in game, delete it.
        if(this->players.size() > 1) 
            resize(playerBeingRemoved); 
    }

    //Removes dissconected player while maintaining contiguous array
    void Game::resize(unsigned int& removedIndex)
    {
        if(removedIndex != this->players.size() - 1) //TO-DO check index out of bounds
        {
            std::swap(this->players[removedIndex], this->players[this->players.size() - 1]);
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

    std::string Game::getPartyFormattedString()
    {
        std::string result;
        //Format | after each player, but not last (helps with client side string split)
        for(unsigned int i = 0; i < this->players.size(); i++)
        {
            if(i != this->players.size() -1)
                result += this->players[i]->toDelimitedString() + "|";
            else
                result += this->players[i]->toDelimitedString();
        }
        return result;
    }

    unsigned int Game::getPartySize() const
    {
        return this->players.size();
    }

    std::string Game::processInstruction(const std::vector<std::string>& instruction)
    {
    
        if(instruction[2] == "c") //c == changed state
        {
            this->stateManager->processStateChange(instruction);
        }
        else if(instruction[2] == "p")  //p == process instruction of the state in progress
        {
           this->stateManager->getCurrentState()->processInstruction(this->players, instruction);

        }
        return this->stateManager->getCurrentState()->getClientMessage();   
    }
    

    Game::~Game()
    {
        (this->players).clear();
    }

}



