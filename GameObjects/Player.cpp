#include <utility>
#include "Player.h"

namespace RPA
{
    Player::Player(const unsigned int& id, const std::string& name)
    {
        this->clientId = id;
        this->name = name;
    }

    char Player::getClass() const
    {
        return this->p_class;
    }

    const unsigned int Player::getClientId() const
    {
        return this->clientId;
    }

    std::string  Player::getName() const
    {
        return this->name;
    }

    void Player::setClass(const char& _class) 
    {
        this->p_class = _class;
    }

    void Player::setName(const std::string& newName)
    {
        this->name = newName;
    }

    Player::~Player()
    {
        
    }
}