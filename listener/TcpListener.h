#pragma once
#include<WS2tcpip.h>
#include<string>
#pragma comment (lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (4096)
class TCPListener;
typedef void(*MessageRecievedHandler)(TCPListener* listener, int socketId, std::string msg);

class TCPListener {

public:

	TCPListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	~TCPListener();

	void sendMessage(int clientSocket, std::string msg);
	bool init();
	void run();
	void cleanUp();

private:
	std::string _ipAddress;
	int	_port;
	MessageRecievedHandler _messageRecieved;

	SOCKET createSoctet();
	SOCKET waitForConnection(SOCKET listening);

};
