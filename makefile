CC = g++
PREFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -L/usr/lib/x86_64-linux-gnu -lgio-2.0
POSTFLAGS = -lbluetooth -Wall -std=c++14 -pthread

all:	RPA

RPA:	main.o Server.o ClientController.o Server/Server.h Server/ClientController.h\
Player.o GameSession/Player.h Game.o GameSession/Game.h\
StateManager.o CharacterCreation.o\
SocketLibrary/Socket.o SocketLibrary/Socket/Socket.h SocketLibrary/ServerSocket.o SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) main.o Server.o ClientController.o Player.o Game.o StateManager.o CharacterCreation.o SocketLibrary/ServerSocket.o SocketLibrary/Socket.o -o server $(POSTFLAGS)

main.o:	main.cpp Server/Server.h
	$(CC) $(PREFLAGS) -c  main.cpp $(POSTFLAGS)

Server.o:	Server/Server.cpp Server/Server.h
	$(CC) $(PREFLAGS) -c Server/Server.cpp $(POSTFLAGS)

ClientController.o: Server/ClientController.cpp Server/ClientController.h
	$(CC) $(PREFLAGS) -c Server/ClientController.cpp $(POSTFLAGS)
	
CharacterCreation.o:	GameSession/GameStates/CharacterCreation.cpp GameSession/GameStates/CharacterCreation.h GameSession/GameStates/GameState.h
	$(CC) $(PREFLAGS) -c GameSession/GameStates/CharacterCreation.cpp $(POSTFLAGS)

StateManager.o:	GameSession/GameStates/StateManager.cpp GameSession/GameStates/StateManager.h
	$(CC) $(PREFLAGS) -c Game.o GameSession/GameStates/StateManager.cpp $(POSTFLAGS)

Game.o:	GameSession/Game.cpp GameSession/Game.h
	$(CC) $(PREFLAGS) -c GameSession/Game.cpp $(POSTFLAGS)

Player.o:	GameSession/Player.cpp GameSession/Player.h
	$(CC) $(PREFLAGS) -c GameSession/Player.cpp $(POSTFLAGS)



clean:
	rm *.o

rebuild: clean RPA