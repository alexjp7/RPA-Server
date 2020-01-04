CC = g++
PREFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -L/usr/lib/x86_64-linux-gnu -lglib-2.0 -L/usr/lib/x86_64-linux-gnu -lgio-2.0
POSTFLAGS = -lbluetooth -Wall -std=gnu++11 -pthread

all:	RPA

RPA:	GameObjects/Player.o GameObjects/Game.o SocketLibrary/Socket.o SocketLibrary/Socket/Socket.h SocketLibrary/ServerSocket.o SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) SocketLibrary/ServerSocket.o SocketLibrary/Socket.o GameObjects/Player.o GameObjects/Game.o server.cpp -o server $(POSTFLAGS)
	
Game.o:	GameObjects/Game.cpp  GameObjects/Game.h GameObjects/Player.o
	$(CC) $(PREFLAGS) -c GameObjects/Game.cpp  Player.o$(POSTFLAGS)

Player.o:	GameObjects/Player.cpp GameObjects/Player.h 
	$(CC) $(PREFLAGS) -c GameObjects/Player.cpp $(POSTFLAGS)

Socket.o:	SocketLibrary/Socket/Socket.cpp SocketLibrary/Socket/Socket.h
	$(CC) $(PREFLAGS) -c Socket/Socket.cpp $(POSTFLAGS)

ServerSocket.o:	SocketLibrary/ServerSocket/ServerSocket.cpp SocketLibrary/ServerSocket/ServerSocket.h
	$(CC) $(PREFLAGS) -c SocketLibrary/ServerSocket/ServerSocket.cpp $(POSTFLAGS)

clean:
	rm *.o

rebuild: clean RPA