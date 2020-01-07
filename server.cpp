
#include <iostream>
#include <fstream>
#include <thread>
#include <exception>
#include <stdexcept>
#include <utility>
#include <assert.h>
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
	kt::Socket* socket;
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
std::mutex client_guard;

std::map<unsigned int, RPA::Game*> games;
std::map<unsigned int, Client> clients;


//Function Prototypes
/*********************************************************************************************/
void serve(const bool& isServing);
void listenForCommand(bool& isServing);
void listenForClient(kt::ServerSocket& server, const bool& isServing);
void pollClient(const bool& isServing);
void removeClient(std::pair<unsigned int, Client> client);
void shutdown();

unsigned int generateClientId(std::string ip, int port);
std::vector<std::string> split(const std::string& original, const char& delim);
void createGame(const unsigned int& clientId, const std::string& name);
void joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name);
void notifyByGame(const unsigned int& gameId, std::string msg);
void notifyClient(const unsigned int& clientId, std::string msg);
void processGameInstructions();
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
	{
		for(auto& game: games)
		{
			delete game.second;
		}
		games.clear();
	}

	if(!clients.empty())
	{
		for(auto& client: clients)
		{
			notifyClient(client.first, "s,server shutdown");
			client.second.socket->close();
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
		client.socket = new kt::Socket(server.accept());
		clients.insert( std::pair<unsigned int,Client>(generateClientId(client.socket->getAddress(), client.socket->getPort()), client));
		std::cout<<" Client CONNECTED -  Clients connected: " << clients.size() << std::endl;
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
				if(!client.second.socket->send("a\n")  )
				{
					client.second.m.lock();
					removeClient(client);
					std::cout<<" Client DISSCONNECTED - Clients Connected:: " << clients.size() << std::endl;
				}
				
			}
		}
	}
}

void removeClient(std::pair<unsigned int, Client> client)
{
	if(client.second.hasGame)
	{
		RPA::Game* gameLeaving = games[client.second.gameId];
		notifyByGame(client.second.gameId,"d, " + gameLeaving->getPlayer(client.first)->getName());
		//gameLeaving->removePlayer(client.first); 
	}

	//clients.erase(client.first);
	//client.second.socket->close();

}



/* Main serving function running on main thread */
void serve(const bool& isServing)
{
	try
	{
		unsigned int gameId;
		for(auto& client: clients)
		{
			if(client.second.socket->ready())
			{
				std::string recieved = client.second.socket->receiveToDelimiter(READ_DELIM);
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
						joinGame(gameId, client.first, clientMessage[1]);
						client.second.hasGame = true;
						client.second.gameId = gameId;
						break;

					case 'i': //Client is in game, and instructions relate to a particualr state of the game 
						processGameInstructions();
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
void notifyByGame(const unsigned int& gameId, std::string msg)
{
	//std::vector<std::unique_ptr<RPA::Player> > connectedPlayers = &games[gameId]->getAllPlayers();
	for(auto& players: games[gameId]->getAllPlayers())
	{
		clients[players->getClientId()].socket->send(msg+"\n");
	}
}

void notifyClient(const unsigned int& clientId, std::string msg)
{
	clients[clientId].socket->send(msg+"\n");
}

/*********************************************************** 
					GAME  FUNCTIONS
************************************************************/
void createGame(const unsigned int& client, const std::string& name)
{
	RPA::Game* game = new RPA::Game(client, name);
	games.insert(std::pair<unsigned int, RPA::Game*>(client, game)); 
	notifyByGame(game->getId(), "c," + name); //TO-DO - fix test message
}

void joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name)
{
	std::cout << " * Join gamed selected" << std::endl;
	std::map<unsigned int, RPA::Game*>::iterator mapIterator;
	mapIterator = games.find(gameId);
	if(mapIterator == games.end()) 
	{
		notifyClient(clientId,"Game ID was not valid");
	}	
	else
	{
		if(games[gameId]->addPlayer(clientId, name))
		{
			notifyByGame(gameId, "c," + name);
		}
		else
		{
			notifyClient(clientId,"game full");
			removeClient(std::pair<unsigned int, Client>(clientId, clients[clientId]));
		}
		
	}
}

void processGameInstructions()
{
	std::cout << "Player is in game" << std::endl;
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