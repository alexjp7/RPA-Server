#pragma once

#include <thread>
#include <exception>
#include <stdexcept>
#include <utility>
#include <vector>

#include <functional>
#include <memory>
#include <chrono>
#include <map>
#include <functional>
#include <string>
#include <signal.h>

//Socket Library
#include "../SocketLibrary/ServerSocket/ServerSocket.h"
#include "../SocketLibrary/SocketExceptions/SocketException.hpp"
#include "../SocketLibrary/SocketExceptions/BindingException.hpp"
#include "../SocketLibrary/SocketExceptions/TimeoutException.hpp"

//Server objects
#include "ClientController.h"

//Game Objects
#include "../GameSession/Game.h"
#include "../GameSession/Player.h"


namespace RPA
{
    class Server
    {
        private: 
            const int PORT = 10001;
            const unsigned int SOCKET_TIME_OUT = 2000000; //2 seconds
            const char READ_DELIM = '\n';    //Delimiter for incoming data
            bool isServing;
            
            kt::ServerSocket serverSocket;

            std::unique_ptr<std::thread> commandListener;
            std::unique_ptr<std::thread> clientListener;
            std::unique_ptr<std::thread> clientPollThread;
            std::map<unsigned int, std::unique_ptr<RPA::Game> >  games;

        public:
            Server();
            ~Server();
            void start();
            void shutdown();

        private:
            RPA::ClientController clientController;

            void listenForCommand();
            void serve();
            void createGame(const unsigned int& clientId, const std::string& name);
            bool joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name);
            void processGameInstructions(const std::vector<std::string>&  clientMessage);
            std::vector<std::string> split(const std::string& original, const char& delim);
            void printClients();
            void printGames();
    };
}