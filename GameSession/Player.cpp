#include <utility>
#include "Player.h"

namespace RPA
{
    Player::Player(const unsigned int& id, const std::string& name, bool isPartyLeader)
    {
        this->clientId = id;
        this->name = name;
        this->adventuringClass = -1;
        this->ready = false;
        this->isPartyLeader = isPartyLeader;
        
    }

    //Client ID
    unsigned int Player::getClientId() const { return this->clientId;}
    //Player Name
    std::string  Player::getName() const { return this->name;}
    void Player::setName(const std::string& newName) { this->name = newName;}
    //Client Class
    int Player::getClass() const{ return this->adventuringClass;}
    void Player::setClass(const unsigned int& _class) {this->adventuringClass = _class;}
    void Player::setPartyLeader(bool isPartyLeader) {this->isPartyLeader = isPartyLeader;}
    //Ready Status
    bool Player::isReady() const{ return this->ready;}
    void Player::toggleReady(){ this->ready = !this->ready;} 
    Player::~Player() { }

    //json structure used when serialising player objects
    json Player::toJson() const
    {
        return json {
            {"id", clientId},
            {"name", name}, 
            {"adventuringClass", adventuringClass},
            {"ready", ready}, 
            {"isPartyLeader", isPartyLeader}
        };
    }

    //For test prints
    std::string Player::toDelimitedString() { return toJson().dump();}
  
    //Overload for serialization method
    void to_json(json& j, const std::unique_ptr<Player>& p)
    {
        j = p->toJson();
    }
}