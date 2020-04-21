#pragma once

//To-do add more when more states are created
namespace RPA
{
    enum class ConnectionInstruction
    {
        NEW_GAME = 0,
        JOIN_GAME = 1,
        DISCONNECTION = 2,
    };
    enum class CreationInstruction
    {
        CONNECTION = 0, 
        DISCONNECTION = 1,
        CLASS_CHANGE = 2,
        READY_UP = 3,
        GAME_START = 4,
    };

    enum class BattleStateInstruction
    {
        DISCONNECTION = 0
    };
}