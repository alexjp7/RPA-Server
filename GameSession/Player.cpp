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

    //Client ID
    unsigned int Player::getClientId() const { return this->clientId;}
    //Player Name
    std::string  Player::getName() const { return this->name;}
    void Player::setName(const std::string& newName) { this->name = newName;}
    //Client Class
    int Player::getClass() const{ return this->adventuringClass;}
    void Player::setClass(const unsigned int& _class) {this->adventuringClass = _class;}
    //Ready Status 
    bool Player::isReady() const{ return this->ready;}
    void Player::toggleReady(){ this->ready = !this->ready;} 
    Player::~Player() { }

    //For test prints
    std::string Player::toDelimitedString() { return  json{{"id", this->getClientId()},{"name", this->getName()}, {"adventuringClass", this->getClass()}, {"ready",this->isReady()}}.dump();}
  
    //Overload for serialization method
    void to_json(json& j, const std::unique_ptr<Player>& p)
    {
        j = json{{"id", p->getClientId()},{"name", p->getName()}, {"adventuringClass", p->getClass()}, {"ready",p->isReady()}};
    }
}