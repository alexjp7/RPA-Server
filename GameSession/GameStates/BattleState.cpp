#include "BattleState.h"
namespace RPA
{
    BattleState::BattleState() : GameState()
    {
        this->stateId = RPA::State::BATTLE_STATE;
        this->originMessage = false;
    }

    /******************************************************
    @todo Add Battle State instructions!
    *******************************************************/
    void BattleState::processInstruction(const std::vector<std::unique_ptr<RPA::Player> >& players, const std::string& message)
    {
        RPA::BattleStateMessage clientMessage(message);

        this->returnedClientMessage = message;
    } 

    std::string BattleState::getClientMessage(){ return this->returnedClientMessage;}
    BattleState::~BattleState() {}
}
