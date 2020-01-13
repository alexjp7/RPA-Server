#include <utility>
#include "Player.h"

namespace RPA
{
    Player::Player(const unsigned int& id, const std::string& name, const unsigned int& partyId)
    {
        this->clientId = id;
        this->name = name;
        this->partyId = partyId;
    }

    unsigned int Player::getClass() const
    {
        return this->playerClass;
    }

    unsigned int Player::getClientId() const
    {
        return this->clientId;
    }

    unsigned int Player::getPartyId() const
    {
        return this->partyId;
    }

    std::string  Player::getName() const
    {
        return this->name;
    }

    void Player::setClass(const unsigned int& _class) 
    {
        this->playerClass = _class;
    }

    void Player::setName(const std::string& newName)
    {
        this->name = newName;
    }

    Player::~Player()
    {
        
    }
}