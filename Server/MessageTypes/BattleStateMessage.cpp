#pragma once
#include "BattleStateMessage.h"


namespace RPA
{
    BattleStateMessage::BattleStateMessage(const std::string& jsonString)
    {

    }

    void BattleStateMessage::deserialize(const std::string& jsonString)
    {

    }
    void BattleStateMessage::serialize(const std::vector<std::unique_ptr<RPA::Player> >& player)
    {

    }

    RPA::BattleStateInstruction BattleStateMessage::getInstruction() {return this->instructionType;}
}