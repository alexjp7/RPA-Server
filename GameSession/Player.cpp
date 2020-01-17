#include <utility>
#include "Player.h"

namespace RPA
{
    Player::Player(const unsigned int& id, const std::string& name)
    {
        this->clientId = id;
        this->name = name;
        this->adventuringClass = -1;
        this->ready = false;
    }

    int Player::getClass() const
    {
        return this->adventuringClass;
    }

    unsigned int Player::getClientId() const
    {
        return this->clientId;
    }

    std::string  Player::getName() const
    {
        return this->name;
    }

    bool Player::isReady() const
    {
        return this->ready;
    }

    void Player::setClass(const unsigned int& _class) 
    {
        this->adventuringClass = _class;
    }

    void Player::setName(const std::string& newName)
    {
        this->name = newName;
    }

    void Player::toggleReady()
    {
        this->ready = !this->ready;
    }
    std::string Player::toDelimitedString()
    {
        return std::to_string(this->clientId) + ":" + this->name + ":" + std::to_string(this->adventuringClass) + ":" + std::to_string(this->ready);
    }


    Player::~Player()
    {
        
    }
}