CC = g++
PREFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -L/usr/lib/x86_64-linux-gnu -lgio-2.0
POSTFLAGS = -lbluetooth -Wall -std=c++14 -pthread

all:	RPA

RPA:	Player.o Game.o StateManager.o CharacterCreation.o GameSession/Game.h GameSession/Player.h SocketLibrary/Socket.o SocketLibrary/Socket/Socket.h SocketLibrary/ServerSocket.o SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) Player.o Game.o CharacterCreation.o StateManager.o SocketLibrary/ServerSocket.o SocketLibrary/Socket.o server.cpp -o server $(POSTFLAGS)
	
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