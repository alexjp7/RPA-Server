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
        this->partyLeaderId = gameId;
        auto manager = std::make_unique<RPA::StateManager>(); //Pushes character creation state 
        this->stateManager = std::move(manager);
        this->addPlayer(playerId, name); ////id, display name, is party leader
    }

    bool Game::addPlayer(const unsigned int& playerId, const std::string& name)
    {
        if(this->players.size() == PARTY_LIMIT) {return false;}

        //First player of a game is the default party leader
        auto player = std::make_unique<Player>(playerId, name, this->players.size() == 0);
        this->players.push_back(std::move(player));

        return true;
    }

    //Remove client from game - typically called upon dissconection
    void Game::removePlayer(const unsigned int& playerId)
    {
        unsigned int playerBeingRemoved = -1;
        if(this->players.size() == 1)
        {
              playerBeingRemoved = 0;
        }
        else
        {
            for (unsigned int i = 0; i < this->players.size(); i++)
            {
                if(this->players[i]->getClientId() == playerId)
                {
                    playerBeingRemoved = i;
                }
            }
        }

        resize(playerBeingRemoved);
        
        //Reset party leader first joined player from remaining players
        if(this->players.size()  > 0)
        {

            this->partyLeaderId = this->players[0]->getClientId(); 
            this->players[0]->setPartyLeader(true);
        }
    }

    //Removes dissconected player while maintaining contiguous array
    void Game::resize(unsigned int& removedIndex)
    {
        if(removedIndex < 0 || removedIndex > 3) return; //Invalid player found
        if(removedIndex != this->players.size() - 1 && this->players.size() > 1) 
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
    
    std::string Game::recieveInstruction(const std::string& clientMessage)
    {
        //Process client state instruction
        const auto& currentGameState =  stateManager->getCurrentState(); 
        currentGameState->processInstruction(players, clientMessage);
        std::string instruction = currentGameState->getClientMessage();
        
        //Check for any state changes iniaited from client
        if(currentGameState->hasRecievedStateChanged())
        {
            stateManager->addState(currentGameState->getNextState(), 
                                   currentGameState->isRemovingPrevious());
        }

        return instruction;
    }

    //Test print out
    void Game::printPlayers() const
    {
        for(auto& p: this->players)
            std:: cout << p->toDelimitedString() << std::endl;
    }

    Game::~Game()
    {
        (this->players).clear();
    }

    //Player Party 
    unsigned int Game::getPartySize() const { return this->players.size();}
    std::vector<std::unique_ptr<Player> > const& Game::getAllPlayers() const{ return this->players;}
    //Game Id
    const unsigned int Game::getId() const { return this->gameId;}
    //Game States
    RPA::State Game::getStateId() const { this->stateManager->getCurrentState()->getStateId();}
    std::string Game::getStateTypeId() const {return typeid(*(this->stateManager->getCurrentState())).name();}
    //Flag to determine whether the instruction that was processed included a message for origin client (sender)
    bool Game::hasOriginMessage() const { return this->stateManager->getCurrentState()->hasOriginMessage(); }
}