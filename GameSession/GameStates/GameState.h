/*---------------------------------------------------------------
                        GAMESTATE
 ---------------------------------------------------------------*/
/***************************************************************
* Abstract model for allowing polymorphic/generic handling
  between the various game states. 

* Encloses functionality that updates any player/game values
  occuring in the client side application
**************************************************************/
#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../Player.h"
#include "../../Enums/GameState.cpp"
#include "../../Enums/MessageType.cpp"

namespace RPA
{
    class GameState
    {
        protected:

            RPA::State stateId;
            RPA::State nextState;
            std::string returnedClientMessage;

            std::unique_ptr<Player> const&  getPlayer(const std::vector<std::unique_ptr<RPA::Player> >& players, const unsigned int& playerId);
            //Flags
            bool originMessage; 
            bool isRemoving; 
            bool hasNextState; 

        public:
            GameState();
            virtual ~GameState(){}

            /***************************************************************
            * Overriden handler for state-specific instrucitons
            @param players -  Reference to the game's players to enable player 
            mutations to reflect changing game state
            @param message - JSON instruction sent from client.
            **************************************************************/
            virtual void processInstruction(const std::vector<std::unique_ptr<RPA::Player> >&players, const std::string& message) = 0; 
          
            /******************************************************
                                    ACCESSORS
            ///////////////////////////////////////////////////////
            @return the output of game state instructions.
             Is used to send back to client
            *******************************************************/
            virtual std::string getClientMessage() = 0;

            /******************************************************
            @return Enumerated game state identifer as defined in
            Enums/GameState.
            *******************************************************/
            RPA::State getStateId() const;

            /******************************************************
            @return State ID for next state 
            *******************************************************/
            RPA::State getNextState() const;


            /******************************************************
            @return Flag for if the server needs to send a message back 
            to the origin client (i.e. the client who sent the message)
            *******************************************************/
            bool hasOriginMessage() const;

            /******************************************************
            @return Whether client is moving from a game state to 
            another, and does not need previous one
            *******************************************************/
            bool isRemovingPrevious() const;

            /******************************************************
            @return Whether the has recieved an instruction to 
            updadte/change the current game state.
            *******************************************************/
            bool hasRecievedStateChanged() const;


    };
}
