#include <iostream>
#include "ClientController.h"

namespace RPA
{
    ClientController::ClientController(){} //Do nothing

	void ClientController::listenForClient(kt::ServerSocket& server, const bool& isServing)
	{
		Client client;
		while(isServing)
		{
			try
			{
				client.socket = kt::Socket(server.accept(POLL_TIME));
				clients.insert( std::pair<unsigned int,Client>(this->generateClientId(client.socket.getAddress(), client.socket.getPort()), client));
				std::cout <<" Client CONNECTED -  Clients connected: " << clients.size() << std::endl;
			} //catch exception in order to cease thread block caused by server.accept(), and force isServing to be re-evaluated
			catch (const kt::TimeoutException &e){} 
		}
	} 
	
	/* Polls Clients for connection, removes dissconencted  clients from map */
	void ClientController::pollClient(const bool& isServing, std::map<unsigned int, std::unique_ptr<RPA::Game> >&  games)
	{
		while(isServing)
		{
			std::this_thread::sleep_for(std::chrono::seconds(POLL_TIME));
			if(!this->clients.empty())
			{
				for(auto& client: this->clients)
				{
					if(!client.second.socket.send("a\n"))
					{
						client.second.m.lock();
						this->removeClient(client.first, games);
						std::cout <<" Client DISSCONNECTED - Clients Connected:: " << clients.size() << std::endl;
					}
				}
			}
		}
	}

	void ClientController::removeClient(const unsigned int& clientId,  std::map<unsigned int, std::unique_ptr<RPA::Game> >& games ,std::string msg)
	{
		Client* clientRemoving = &clients[clientId];
		notifyClient(clientId, "d");

		//Remove client from game if in one
		if(clientRemoving->hasGame)
		{
			int gameId = clientRemoving->gameId;
			games[gameId]->removePlayer(clientId); 
			if(games[gameId]->getPartySize() == 0) //Remove game if no players are connected to it
			{
				games.erase(gameId); 
			} 
			else//notify remaining players
			{
				notifyGame(RPA::ConnectionMessage(clientId, State::CHARACTER_CREATION, ConnectionInstruction::DISCONNECTION).getGameMessage(), games[gameId]); 
			}
		}
		clientRemoving->socket.close();
		clients.erase(clientId);
	}

	void ClientController::notifyGame(const std::string msg,  const std::unique_ptr<RPA::Game>& game)
	{
		for(auto& players: game->getAllPlayers())
			clients[players->getClientId()].socket.send(msg+"\n");
	}

	void ClientController::notifyGame(const std::string msg, const std::unique_ptr<RPA::Game>& game, const unsigned int& originClient)
	{
		for(auto& players: game->getAllPlayers())
		{
			if(originClient != players->getClientId())
				clients[players->getClientId()].socket.send(msg+"\n");
		}
	}

	void ClientController::notifyClient(const unsigned int& clientId, const std::string& msg = "")
	{
		if(msg == "") return;
		clients[clientId].socket.send(msg+"\n");
	}

	void ClientController::setGameStatus(const unsigned int& clientId, const unsigned& gameId, const std::string& msg )
	{
		clients[clientId].hasGame = true;
		clients[clientId].gameId = gameId;
		this->notifyClient(clientId, msg); //send game id back to client who created game
	}

	static int temp = 0; //FOR TESTING
	/* Generates a hashed ID of the client IP and Port*/
	unsigned int ClientController::generateClientId(std::string ip, int port)
	{
		//std::hash<std::string> hasher;
		std::string result = "";
		for(char c: ip)
		{
			if(c != '.')
				result += c;
		}
		result += port;
		return ++temp;
	}
	
	bool ClientController::hasClients() const {return !this->clients.empty();}
	std::map<unsigned int, Client>& ClientController::getClients()  {return this->clients;}
} // namespace RPA


