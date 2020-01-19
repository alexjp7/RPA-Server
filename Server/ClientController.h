#pragma once

#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <thread>

#include "../Cpp-SocketLibrary/ServerSocket/ServerSocket.h"
#include "../Cpp-SocketLibrary/SocketExceptions/SocketException.hpp"
#include "../Cpp-SocketLibrary/SocketExceptions/BindingException.hpp"
#include "../Cpp-SocketLibrary/SocketExceptions/TimeoutException.hpp"

#include "../GameSession/Player.h"
#include "../GameSession/Game.h"

namespace RPA
{
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

    class ClientController
    {
        private:
            const unsigned int POLL_TIME = 2; //Time between client polling
            std::map<unsigned int, Client> clients;
        public:
            ClientController();
            //Client threads
            void listenForClient(kt::ServerSocket& server, const bool& isServing);
            void pollClient(const bool& isServing, std::map<unsigned int, std::unique_ptr<RPA::Game> >&  games);
            //Client communication 
            void notifyGame(const unsigned int& gameId, std::string msg, std::map<unsigned int, std::unique_ptr<RPA::Game> >& games);
            void notifyGame(const unsigned int& gameId, std::string msg, const unsigned int& originClient,  std::map<unsigned int, std::unique_ptr<RPA::Game> >& games);
            void notifyClient(const unsigned int& clientId, std::string msg);
            void removeClient(const unsigned int& clientId, std::map<unsigned int, std::unique_ptr<RPA::Game> >&  games, std::string msg = "");

        private:
            unsigned int generateClientId(std::string ip, int port);
            
        public:
            //Client getters
            bool hasClients() const;
            std::map<unsigned int, Client>& getClients();
    };
} // namespace RPA
