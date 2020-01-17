#include <iostream>
#include <thread>
#include <exception>
#include <stdexcept>
#include <utility>
#include <vector>
#include <mutex>
#include <functional>
#include <memory>
#include <chrono>
#include <map>
#include <random>
#include <functional>
#include <string>
#include <signal.h>
//Socket Library
#include "SocketLibrary/Socket/Socket.h"
#include "SocketLibrary/ServerSocket/ServerSocket.h"
#include "SocketLibrary/SocketExceptions/SocketException.hpp"
#include "SocketLibrary/SocketExceptions/BindingException.hpp"
#include "SocketLibrary/SocketExceptions/TimeoutException.hpp"

//Game Objects
#include "GameSession/Game.h"
#include "GameSession/Player.h"

//Time between client polling
const unsigned int POLL_TIME = 2;
const unsigned int SOCKET_TIME_OUT = 2000000; //2 seconds
//Delimiter for incoming data
const char READ_DELIM = '\n';

struct Client 
{
	std::mutex m;
	kt::Socket socket;
	bool hasGame;
	unsigned int gameId;
	Client() : hasGame(false), gameId(-1){}

	Client(const Client& other)
	{
		this->socket = other.socket;
		this->hasGame = other.hasGame;
		this->gameId = other.gameId;
	}
};

std::map<unsigned int, std::unique_ptr<RPA::Game> > games;
std::map<unsigned int, Client> clients;

//Function Prototypes
/*********************************************************************************************/
void serve(const bool& isServing);
void listenForCommand(bool& isServing);
void listenForClient(kt::ServerSocket& server, const bool& isServing);
void pollClient(const bool& isServing);
void removeClient(const unsigned int& clientId, std::string msg = "");
void shutdown();

void createGame(const unsigned int& clientId, const std::string& name);
bool joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name);
void processGameInstructions(const std::vector<std::string>&  clientMessage);
void notifyGame(const unsigned int& gameId, std::string msg);
void notifyGame(const unsigned int& gameId, std::string msg, const unsigned int& originClient);
void notifyClient(const unsigned int& clientId, std::string msg);
unsigned int generateClientId(std::string ip, int port);
std::vector<std::string> split(const std::string& original, const char& delim);
/*********************************************************************************************/

int main()
{
	//Ignores 'broken' socket connection signal
	signal(SIGPIPE, SIG_IGN);
	bool isServing = true;
	std::cout<<"STARTING SERVER\n-------------------"<<std::endl;
	try
	{	//Initialize Server Socket, client listener and client poller threads
		kt::ServerSocket server(kt::SocketType::Wifi, 10001);
		std::thread commandListener (listenForCommand, std::ref(isServing));
		std::thread clientListener (listenForClient, std::ref(server), std::ref(isServing));
		std::thread clientPollThread(pollClient, std::ref(isServing));

		//Serve while a client connection is maintained
		while(isServing)
		{
			if(!clients.empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				serve(isServing);
			}
		}
		commandListener.join();
		clientListener.join();
		clientPollThread.join();
		shutdown();
		server.close();
	}
	catch(std::runtime_error ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return 0;
}

/*********************************************************** 
				SERVER FUNCTIONS/CLIENT LISTENERS
************************************************************/
//Allows for commands to be given to server via command line
void listenForCommand(bool& isServing)
{
	std::string instruction;
	while (isServing)
	{
		std::cin>>instruction;
		if(instruction == "shutdown")
		{	
			isServing = false;
		}
	}
}

//Terminates all connections, games and clients
void shutdown()
{
	std::cout<<"STOPPING SERVER\n-------------------"<<std::endl;
	if(!games.empty())
		games.clear();

	if(!clients.empty())
	{
		for(auto& client: clients)
		{
			notifyClient(client.first, "s,server shutdown");
			client.second.socket.close();
		}
		clients.clear();
	}
}

/* Listens for client connections*/
void listenForClient(kt::ServerSocket& server, const bool& isServing)
{
	Client client;
	while(isServing)
	{
		try
		{
			client.socket = kt::Socket(server.accept(POLL_TIME));
			clients.insert( std::pair<unsigned int,Client>(generateClientId(client.socket.getAddress(), client.socket.getPort()), client));
			std::cout<<" Client CONNECTED -  Clients connected: " << clients.size() << std::endl;
		} //catch exception in order to cease thread block caused by server.accept(), and force isServing to be re-evaluated
		catch (const kt::TimeoutException &e){} 

	}
} 

/* Polls Clients for connection, removes dissconencted  clients from map */
void pollClient(const bool& isServing)
{
	while(isServing)
	{
		std::this_thread::sleep_for(std::chrono::seconds(POLL_TIME));
		if(!clients.empty())
		{
			for(auto& client: clients)
			{
				if(!client.second.socket.send("a\n"))
				{
					client.second.m.lock();
					removeClient(client.first);
					std::cout<<" Client DISSCONNECTED - Clients Connected:: " << clients.size() << std::endl;
				}
			}
		}
	}
}

void removeClient(const unsigned int& clientId, std::string msg)
{
	Client* clientRemoving = &clients[clientId];
	if(msg != "") {notifyClient(clientId, msg);}

	//Remove client from game if in one
	if(clientRemoving->hasGame)
	{
		int gameId = clientRemoving->gameId;
		games[gameId]->removePlayer(clientId); 

		//Remove game if no players are connected to it
		if(games[gameId]->getPartySize() == 0)
		{
			games.erase(gameId);
		}
		else //Notify  remaining players of party change
		{
			notifyGame(gameId, "d," + std::to_string(clientId));
		}
	}
	clientRemoving->socket.close();
	clients.erase(clientId);
}

/* Main serving function running on main thread */
void serve(const bool& isServing)
{
	try
	{
		unsigned int gameId;
		for(auto& client: clients)
		{
			if(client.second.socket.ready())
			{
				std::string recieved = client.second.socket.receiveToDelimiter(READ_DELIM);
				if(recieved[0] == '\0') return;
				std::vector<std::string> clientMessage = split(recieved,',');
				switch (clientMessage[0][0])
				{
					case 'n': //New Game, game object needs to be created
						createGame(client.first, clientMessage[1]);
						client.second.hasGame = true;
						client.second.gameId = client.first;
						std::cout <<" * Game created With Id: "<<games[client.first]->getId() << std::endl;
						break;
						
					case 'j': //Joining  Game, id from existing game needs to be accepted
						gameId = (unsigned int)std::stoi(clientMessage[2]);
						if(joinGame(gameId, client.first, clientMessage[1]))
						{
							std::string message =  "g," + std::to_string(client.first) + "," + games[gameId]->getPartyFormattedString();
							client.second.hasGame = true;
							client.second.gameId = gameId;
							notifyClient(client.first, message);
							notifyGame(gameId, "c," + clientMessage[1] + "," + std::to_string(client.first), client.first);
						}
						else
						{
							removeClient(client.first, "game id was invalid or game is full");
						}
						break;

					case 'i': //Client is in game, and instructions relate to a particualr state of the game 
						std::cout << "FROM CLIENT:" << recieved << std::endl;
						processGameInstructions(clientMessage);
						break;

					default:
						std::cout << "went in default" << std::endl;
						break;
				}
			}
		}
	}
	catch (const kt::SocketException& se)
	{
		std::cout << se.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "CAUGHT IT" << std::endl;
	}
}

/*********************************************************** 
				CLIENT SEND FUNCTIONS
************************************************************/
//Sends to all connected player clients for a given game
void notifyGame(const unsigned int& gameId, std::string msg)
{
	for(auto& players: games[gameId]->getAllPlayers())
		clients[players->getClientId()].socket.send(msg+"\n");
}

//Overload - Disregards sending message to origin client 
void notifyGame(const unsigned int& gameId, std::string msg, const unsigned int& originClient)
{
	for(auto& players: games[gameId]->getAllPlayers())
	{
		if(originClient != players->getClientId())
			clients[players->getClientId()].socket.send(msg+"\n");
	}
}

void notifyClient(const unsigned int& clientId, std::string msg)
{
	clients[clientId].socket.send(msg+"\n");
}

/*********************************************************** 
					GAME  FUNCTIONS
************************************************************/
void createGame(const unsigned int& client, const std::string& name)
{
	auto game = std::make_unique<RPA::Game>(client, name);
	games.insert(std::pair<unsigned int, std::unique_ptr<RPA::Game> >(client, std::move(game))); 
	notifyClient(client, "g," + std::to_string(client)); //send game id back to client who created game
}

bool joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name)
{
	std::cout << " * Join gamed selected" << std::endl;
	if(!games.empty())
	{
		std::map<unsigned int, std::unique_ptr<RPA::Game> >::iterator mapIterator;
		mapIterator = games.find(gameId);
		if(mapIterator != games.end()) 
		{
			if(games[gameId]->addPlayer(clientId, name))
				return true;
		}
	}
	return false;
}

void processGameInstructions(const std::vector<std::string>& clientMessage)
{
	unsigned int gameId = std::stoi(clientMessage[1]);
	// 0 = server instruction
	// 1 = gameId
	// 2 = state manager instruction
	// 3 = origin client id
	// 4+ =  state instruction
	std::string serverReply = games[gameId]->processInstruction(clientMessage);
	std::cout << "serverReply: "<< serverReply << std::endl;
	notifyGame(gameId,serverReply, (unsigned int)std::stoi(clientMessage[3]));

}

/*********************************************************** 
  						AUXILLARY
************************************************************/
static int temp = 0; //REMEMBER TO NOT FORGET ABOUT this
/* Generates a hashed ID of the client IP and Port*/
unsigned int generateClientId(std::string ip, int port)
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

std::vector<std::string> split(const std::string& original, const char& delim)
{
	std::vector<std::string> result;
	result.reserve(3);
	std::string element = "";
	for(char c: original)
	{
		if(c == delim)
		{
			result.push_back(element);
			element = "";
		}
		else
		{
			element += c;
		}
		
	}
	result.push_back(element);
	return result;
}