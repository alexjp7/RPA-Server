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
				std::cout<<" Client CONNECTED -  Clients connected: " << clients.size() << std::endl;
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
						std::cout<<" Client DISSCONNECTED - Clients Connected:: " << clients.size() << std::endl;
					}
				}
			}
		}
	}

	void ClientController::removeClient(const unsigned int& clientId,  std::map<unsigned int, std::unique_ptr<RPA::Game> >& games ,std::string msg)
	{
		Client* clientRemoving = &clients[clientId];
		if(msg != "") {notifyClient(clientId, msg);}

		//Remove client from game if in one
		if(clientRemoving->hasGame)
		{
			int gameId = clientRemoving->gameId;
			games[gameId]->removePlayer(clientId); 
			if(games[gameId]->getPartySize() == 0) games.erase(gameId); //Remove game if no players are connected to it
			else notifyGame(gameId, "d," + std::to_string(clientId), games); //notify remaining players
		}
		clientRemoving->socket.close();
		clients.erase(clientId);
	}

	void ClientController::notifyGame(const unsigned int& gameId, std::string msg,  std::map<unsigned int, std::unique_ptr<RPA::Game> >&  games)
	{
		for(auto& players: games[gameId]->getAllPlayers())
			clients[players->getClientId()].socket.send(msg+"\n");
	}

	void ClientController::notifyGame(const unsigned int& gameId, std::string msg, const unsigned int& originClient,  std::map<unsigned int, std::unique_ptr<RPA::Game> >& games)
	{
		for(auto& players: games[gameId]->getAllPlayers())
		{
			if(originClient != players->getClientId())
				clients[players->getClientId()].socket.send(msg+"\n");
		}
	}

	void ClientController::notifyClient(const unsigned int& clientId, std::string msg)
	{
		clients[clientId].socket.send(msg+"\n");
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


