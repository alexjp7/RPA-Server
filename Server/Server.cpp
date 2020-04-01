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
        std::cout << "STARTING SERVER..." << std::endl;
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

    /* Main serving function running on main thread */
    void Server::serve()
    {
        try
        {
            for(auto& client: clientController.getClients())
            {
                if(client.second.socket.ready())
                {
                    std::string recieved = client.second.socket.receiveToDelimiter(READ_DELIM);
                    if(recieved[0] == '\0') return;
                    if(!client.second.hasGame)
                    {   //inbound and outbound messages between client and server
                        RPA::ConnectionMessage inboundMessage(recieved);
                        //Player Creating Game
                        if( inboundMessage.getGameId() == -1) 
                        {
                            createGame(client.first, inboundMessage.getClientName());
                            RPA::ConnectionMessage outboundMessage(client.first, client.first,State::CHARACTER_CREATION, inboundMessage.getClientName(), games[client.first]->getAllPlayers());
                            clientController.setGameStatus(client.first, client.first, outboundMessage.getClientMessage());
                        }
                        else // Player Joining Game
                            if(joinGame(inboundMessage.getGameId(), client.first, inboundMessage.getClientName()) )
                            {   //Send game data to conencting client
                                RPA::ConnectionMessage outboundMessage(inboundMessage.getGameId(), client.first,State::CHARACTER_CREATION,inboundMessage.getClientName(), games[inboundMessage.getGameId()]->getAllPlayers(), ConnectionInstruction::JOIN_GAME);
                                clientController.setGameStatus(client.first, inboundMessage.getGameId(), outboundMessage.getClientMessage());

                                //Send new player data to other players in a game
                                clientController.notifyGame(outboundMessage.getGameMessage(), games[inboundMessage.getGameId()], client.first);
                            }
                            else clientController.removeClient(client.first, games);
                    }
                    else //Process Message Of in-progress game
                    {
                        std::string clientMessage = games[client.second.gameId]->recieveInstruction(recieved);
                        //Checks for if the server needs to send a message back to the origin client (i.e. the client who sent the message)
                        if(games[client.second.gameId]->hasOriginMessage()) clientController.notifyGame(clientMessage, games[client.second.gameId]);
                        else clientController.notifyGame(clientMessage, games[client.second.gameId], client.first);
                    }
                }
            }
        }
        catch (const kt::SocketException& se)
        {
            std::cout << se.what() << std::endl;
        }
        catch (std::exception &e )
        {
            std::cout << "CAUGHT: " << e.what() << std::endl;
        }
    }

    void Server::createGame(const unsigned int& clientId, const std::string& name)
    {
        auto game = std::make_unique<RPA::Game>(clientId, name);
        games.insert(std::pair<unsigned int, std::unique_ptr<RPA::Game> >(clientId, std::move(game)));
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
                {
                    return true;
                }
            }
        }
        return false;
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
        std::cout << "STOPPING SERVER.." << std::endl;

        commandListener->join();
        clientListener->join();
        clientPollThread->join();

        if(!games.empty()) games.clear();
        if(!clientController.hasClients())
        {
            for(auto& client: clientController.getClients())
                client.second.socket.close();
                
            clientController.getClients().clear();
        }
        serverSocket.close();
    }
    
    //Allows for commands to be given to server via command line
    void Server::listenForCommand() //To-do - come up with a more useful data/server instructions
    {
        std::cout << " (-s) Shutdown \n (-c) Clients \n (-g) Games" << std::endl;
        std::string instruction;
        while (this->isServing)
        {
            std::cin>>instruction;
            std::cout << "----------------------\n";
            if(instruction[0] == '-')
            {
                switch (instruction[1])
                {
                    case 's':
                        this->isServing = false;
                        break;
                    case 'c':
                        this->printClients();
                        break;
                    case 'g':
                        this->printGames();
                        break;
                    default:
                        std::cout << "Unknown Command - " << instruction << std::endl;
                        break;
                }
            }
            else std::cout << "Valid Comamnds: (-s) Shutdown \n (-c) Clients \n (-g) Games" << std::endl;
        }
    }

    //prints a list of client IDs to CL
    void Server::printClients()
    {
        std::cout << clientController.getClients().size() << " client/s connected" << std::endl;
    }

    //prints a list of Game IDs to CL
    void Server::printGames()
    {

        std::cout << this->games.size() << " game/s in progress" << std::endl;
        if(this->games.size() == 0) return;
        std::cout << "----------------------\nGames List\n";
        for(auto& g: games)
        {
            std::cout<<"Game ID: " << g.second->getId() << " \n";
        }

        std::string instruction;
        std::cout << "\nEnter Game ID for Details or Enter d to return: ";
        std::cin >> instruction;
        std::cout << "----------------------\n";

        if(instruction[0] != 'd') 
        {
            try
            {
                unsigned int gameId = std::stoi(instruction);

                std::map<unsigned int, std::unique_ptr<RPA::Game> >::iterator mapIterator;
                mapIterator = games.find(gameId);
                if(mapIterator == games.end())
                {
                    std::cout << "Invalid Game Id" << std::endl;
                } 
                else
                {
                    std::cout << "Current State: " << games[gameId]->getStateTypeId() << "\nPlayers Connected " << games[gameId]->getPartySize() << std::endl;
                    for(auto& player : games[gameId]->getAllPlayers())
                    {
                        std::cout <<" Player - " << player->toDelimitedString() << std::endl;
                    }
                }
            }
            catch(std::invalid_argument& e)
            {
                std::cerr << "Game IDs MUST be integers" << std::endl;
            }
        }
        std::cout<< "Returning to server main menu..." << std::endl;
    }

    Server::~Server()
    {
        this->shutdown();
    }
}