CC = g++
PREFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -L/usr/lib/x86_64-linux-gnu -lgio-2.0
POSTFLAGS = -lbluetooth -Wall -std=c++14 -pthread

all:	RPA

RPA:	main.o Server.o ClientController.o Server/Server.h Server/ClientController.h\
Player.o GameSession/Player.h Game.o GameSession/Game.h\
StateManager.o CharacterCreation.o\
Cpp-SocketLibrary/Socket.o Cpp-SocketLibrary/Socket/Socket.h Cpp-SocketLibrary/ServerSocket.o Cpp-SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) bin/main.o bin/Server.o bin/ClientController.o bin/Player.o bin/Game.o bin/StateManager.o bin/CharacterCreation.o Cpp-SocketLibrary/ServerSocket.o Cpp-SocketLibrary/Socket.o -o server $(POSTFLAGS)

main.o:	main.cpp Server/Server.h
	$(CC) $(PREFLAGS) -c  main.cpp -o bin/main.o $(POSTFLAGS)

Server.o:	Server/Server.cpp Server/Server.h
	$(CC) $(PREFLAGS) -c Server/Server.cpp -o bin/Server.o $(POSTFLAGS)

ClientController.o: Server/ClientController.cpp Server/ClientController.h
	$(CC) $(PREFLAGS) -c Server/ClientController.cpp -o bin/ClientController.o $(POSTFLAGS)
	
CharacterCreation.o:	GameSession/GameStates/CharacterCreation.cpp GameSession/GameStates/CharacterCreation.h GameSession/GameStates/GameState.h
	$(CC) $(PREFLAGS) -c GameSession/GameStates/CharacterCreation.cpp -o bin/CharacterCreation.o $(POSTFLAGS)

StateManager.o:	GameSession/GameStates/StateManager.cpp GameSession/GameStates/StateManager.h
	$(CC) $(PREFLAGS) -c GameSession/GameStates/StateManager.cpp -o bin/StateManager.o $(POSTFLAGS)

Game.o:	GameSession/Game.cpp GameSession/Game.h
	$(CC) $(PREFLAGS) -c GameSession/Game.cpp -o bin/Game.o $(POSTFLAGS)

Player.o:	GameSession/Player.cpp GameSession/Player.h
	$(CC) $(PREFLAGS) -c GameSession/Player.cpp -o bin/Player.o $(POSTFLAGS)

#Only need to run once
libs:
	git clone https://github.com/KyleGonzalez/Cpp-SocketLibrary.git/
	chmod -R 775 Cpp-SocketLibrary
	sudo Cpp-SocketLibrary/dependencies.sh
	make -C Cpp-SocketLibrary
	mkdir bin

clean:
	rm bin/*.o

rebuild: clean RPA
