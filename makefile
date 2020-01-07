CC = g++
PREFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -L/usr/lib/x86_64-linux-gnu -lgio-2.0
POSTFLAGS = -lbluetooth -Wall -std=c++14 -pthread

all:	RPA

RPA:	GameSession/Player.o GameSession/Game.o SocketLibrary/Socket.o SocketLibrary/Socket/Socket.h SocketLibrary/ServerSocket.o SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) SocketLibrary/ServerSocket.o SocketLibrary/Socket.o Player.o Game.o server.cpp -o server $(POSTFLAGS)
	
GameSession/Game.o:	GameSession/Game.cpp GameSession/Game.h
	$(CC) $(PREFLAGS) -c GameSession/Game.cpp Player.o $(POSTFLAGS)

GameSession/Player.o:	GameSession/Player.cpp GameSession/Player.h
	$(CC) $(PREFLAGS) -c GameSession/Player.cpp $(POSTFLAGS)

Socket.o:	SocketLibrary/Socket/Socket.cpp SocketLibrary/Socket/Socket.h
	$(CC) $(PREFLAGS) -c Socket/Socket.cpp $(POSTFLAGS)

ServerSocket.o:	SocketLibrary/ServerSocket/ServerSocket.cpp SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) -c SocketLibrary/ServerSocket/ServerSocket.cpp $(POSTFLAGS)

clean:
	rm *.o

rebuild: clean RPA