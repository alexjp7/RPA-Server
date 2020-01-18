#include "Server.h"

namespace RPA
{
    Server::Server()
    {   //Ignores 'broken' socket connection signal
      	signal(SIGPIPE, SIG_IGN);
        serverSocket = kt::ServerSocket(kt::SocketType::Wifi, this->PORT);
        
    }
    //Intialises client listener threads and begins serving
    void Server::start()
    {
        std::cout<<"STARTING SERVER\n-------------------"<<std::endl;
        this->isServing = true;
        try
        {	//Initialize Server Socket, client listener and client poller threads
            commandListener = std::make_unique<std::thread>(&Server::listenForCommand, this);
            clientListener = std::make_unique<std::thread>(&ClientController::listenForClient, &this->clientController ,std::ref(this->serverSocket), std::ref(this->isServing));
            clientPollThread = std::make_unique<std::thread>(&ClientController::pollClient, &this->clientController ,std::ref(this->isServing), std::ref(this->games));

            //Serve while a client connection is maintained
            while(this->isServing)
            {
                if(clientController.hasClients())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    serve();
                }
            }
        }
        catch(std::runtime_error ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    //Allows for commands to be given to server via command line
    void Server::listenForCommand() //To-do - come up with a more useful data/server instructions
    {
        std::string instruction;
        while (this->isServing)
        {
            std::cin>>instruction;
            if(instruction == "shutdown")
            {	
                this->isServing = false;
            }
            else if(instruction == "clients")
            {
                this->printClients();
            }
            else if(instruction == "games")
            {
                this->printGames();
            }
            std::cout << "--------------------" << std::endl;
        }
    }

    /* Main serving function running on main thread */
    void Server::serve()
    {
        try
        {
            unsigned int gameId;
            for(auto& client: clientController.getClients())
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
                            break;
                            
                        case 'j': //Joining  Game, id from existing game needs to be accepted
                            gameId = (unsigned int)std::stoi(clientMessage[2]);
                            if(joinGame(gameId, client.first, clientMessage[1]))
                            {
                                std::string message =  "g," + std::to_string(client.first) + "," + games[gameId]->getPartyFormattedString();
                                client.second.hasGame = true;
                                client.second.gameId = gameId;
                                clientController.notifyClient(client.first, message);
                                clientController.notifyGame(gameId, "c," + clientMessage[1] + "," + std::to_string(client.first), client.first, games);
                            }
                            else
                            {
                                clientController.removeClient(client.first, games,"game id was invalid or game is full");
                            }
                            break;

                        case 'i': //Client is in game, and instructions relate to a particualr state of the game 
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

    void Server::createGame(const unsigned int& client, const std::string& name)
    {
        auto game = std::make_unique<RPA::Game>(client, name);
        games.insert(std::pair<unsigned int, std::unique_ptr<RPA::Game> >(client, std::move(game))); 
        clientController.notifyClient(client, "g," + std::to_string(client)); //send game id back to client who created game
    }

    bool Server::joinGame(const unsigned int& gameId, const unsigned int& clientId, const std::string& name)
    {
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

    void Server::processGameInstructions(const std::vector<std::string>&  clientMessage)
    {
        unsigned int gameId = std::stoi(clientMessage[1]);
        // 0 = server instruction
        // 1 = gameId
        // 2 = state manager instruction
        // 3 = origin client id
        // 4+ =  state instruction
        std::string serverReply = games[gameId]->processInstruction(clientMessage);
        clientController.notifyGame(gameId,serverReply, (unsigned int)std::stoi(clientMessage[3]), games);
    }

    std::vector<std::string> Server::split(const std::string& original, const char& delim)
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
                element += c;

        }
        result.push_back(element);
        return result;
    }


    void Server::shutdown()
    {
        std::cout<<"STOPPING SERVER\n-------------------"<<std::endl;

        commandListener->join();
        clientListener->join();
        clientPollThread->join();

        if(!games.empty())
            games.clear();

        if(!clientController.hasClients())
        {
            for(auto& client: clientController.getClients())
            {
                clientController.notifyClient(client.first, "s,server shutdown");
                client.second.socket.close();
            }
            clientController.getClients().clear();
        }
        serverSocket.close();
    }

    //prints a list of client IDs to CL
    void Server::printClients()
    {
        if(!clientController.hasClients()) {std::cout<<"No Clients Connected" << std::endl; return;}
        std::cout << clientController.getClients().size() << " client/s connected" << std::endl;
    }
    //prints a list of client IDs to CL
    void Server::printGames()
    {
        if(this->games.empty()) {std::cout<<"No Games In Progress" << std::endl; return;}
        std::cout << this->games.size() << " game/s in progress" << std::endl;
    }

    Server::~Server()
    {
        this->shutdown();
    }
    
}