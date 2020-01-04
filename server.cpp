
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

//Socket Library
#include "SocketLibrary/Socket/Socket.h"
#include "SocketLibrary/ServerSocket/ServerSocket.h"
#include "SocketLibrary/SocketExceptions/SocketException.hpp"
#include "SocketLibrary/SocketExceptions/BindingException.hpp"

//Game Objects
#include "GameObjects/Game.h"
#include "GameObjects/Player.h"

//Time between client polling
const unsigned int POLL_TIME = 2;
//Delimiter for incoming data
const char READ_DELIM = '\n';



struct Client 
{
	kt::Socket* socket;
	bool hasGame;
	unsigned int gameId;
	Client() : hasGame(false), gameId(-1){}

};
std::mutex client_guard;

std::map<unsigned int, RPA::Game*> games;
std::map<unsigned int, Client> clients;


//Function Prototypes
/*********************************************************************************************/
void serve(bool& isServing);
void listenForClient(kt::ServerSocket& server);
void pollClient();

unsigned int generateClientId(std::string ip, int port);
std::vector<std::string> split(const std::string& original, const char& delim);
void createGame(const unsigned int& clientId, const std::string& name);
void joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name);
void notifyByGame(const unsigned int& gameId, std::string msg);
void processGameInstructions();
/*********************************************************************************************/

int main()
{
	bool isServering = true;
	std::cout<<"STARTING SERVER\n-------------------"<<std::endl;
	try
	{	//Initialize Server Socket, client listener and client poller threads
		kt::ServerSocket server(kt::SocketType::Wifi, 10001);
		std::thread clientListener (listenForClient, std::ref(server));
		std::thread clientPollThread(pollClient);

		//Serve while a client connection is maintained
		while(isServering)
		{
			if(!clients.empty())
			{
				serve(isServering);
			}
		}
		server.close();
	}
	catch(std::runtime_error ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}

/* Polls Clients for connection, removes dissconencted  clients from vector */
void pollClient()
{
	while(true)
	{
		if(!clients.empty())
		{
			std::this_thread::sleep_for(std::chrono::seconds(POLL_TIME));
			for(auto& client: clients)
			{
				if(!client.second.socket->send("a\n"))
				{
					client_guard.lock();
					if(client.second.hasGame)
						games[client.second.gameId]->removePlayer(client.first);

					clients.erase(client.first);
					client_guard.unlock();
					std::cout<<" Client DISSCONNECTED - Clients Connected:: " << clients.size() << std::endl;
				}
			}
		}
	}
}

/* Listens for client connections*/
void listenForClient(kt::ServerSocket& server)
{
	Client client;
	while(true)
	{
		client.socket = new kt::Socket(server.accept());
		clients.insert( std::pair<unsigned int,Client>(generateClientId(client.socket->getAddress(), client.socket->getPort()), client));
		std::cout<<" Client CONNECTED -  Clients connected: " << clients.size() << std::endl;
	}
} 
/* Main serving function running on main thread */
void serve(bool& isServing)
{
	bool error = false;
	if(clients.empty()) return;
	try
	{
		unsigned int gameId;
		for(auto& client: clients)
		{
			if(client.second.socket->ready())
			{
				std::vector<std::string> clientMessage = split(client.second.socket->receiveToDelimiter(READ_DELIM), ',');
				switch (clientMessage[0][0])
				{
					case 'n': //New Game, game object needs to be created
						createGame(client.first, clientMessage[1]);
						client.second.hasGame = true;
						client.second.gameId = client.first;
						std::cout <<"Game created With Id: "<<games[client.first]->getId() << std::endl;
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
						break;
				}

			}
		}
	}
	catch (const kt::SocketException& se)
	{
		std::cout << se.what() << std::endl;
		error = true;
	}
	catch (...)
	{
		error = true;
		std::cout << "CAUGHT IT" << std::endl;
	}
	assert(error == false);
}

void notifyByGame(const unsigned int& gameId, std::string msg)
{
	std::vector<RPA::Player*> connectedPlayers = games[gameId]->getAllPlayers();
	for(auto players: connectedPlayers)
	{
		clients[players->getClientId()].socket->send(msg+"\n");
	}
}

void createGame(const unsigned int& client, const std::string& name)
{
	RPA::Game* game = new RPA::Game(client, name);
	games.insert(std::pair<unsigned int, RPA::Game*>(client, game)); 
	notifyByGame(game->getId(), "c," + name); //TO-DO - fix test message
}

void joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name)
{
	std::cout << "Join gamed selected" << std::endl;
	std::map<unsigned int, RPA::Game*>::iterator mapIterator;
	mapIterator = games.find(gameId);
	if(mapIterator == games.end()) {return;} //TO-DO deal with client unsucessful game search
	else
	{
		games[gameId]->addPlayer(clientId, name);
		notifyByGame(gameId, "c," + name);
	}
}

void processGameInstructions()
{
	std::cout << "Player is in game" << std::endl;
}

static int temp = 0;
/* Generates a hashed ID of the client IP and Port*/
unsigned int generateClientId(std::string ip, int port)
{

	std::hash<std::string> hasher;
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