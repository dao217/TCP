#include"TcpListener.h"



////////////public////////////

TCPListener::TCPListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	:_ipAddress(ipAddress), _port(port), _messageRecieved(handler)
{

}

TCPListener::~TCPListener()
{
	cleanUp();
}

void TCPListener::sendMessage(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

bool TCPListener::init()
{
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);
	
	return wsInit == 0;
}

void TCPListener::run()
{
	char buf[MAX_BUFFER_SIZE];

	while (true) {
		SOCKET listening = createSoctet();
		if (listening == INVALID_SOCKET) break;
		
		SOCKET client = waitForConnection(listening);
		if (client == INVALID_SOCKET) {
			closesocket(listening);

			int byteReceived = 0;
			do {
				ZeroMemory(buf, MAX_BUFFER_SIZE);
				byteReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);

				if (byteReceived > 0) {
					if (_messageRecieved != nullptr) {
						_messageRecieved(this, client, std::string(buf, 0, byteReceived));
					}
				}
			} while (byteReceived > 0);
			closesocket(client);
		}

	}
}

void TCPListener::cleanUp()
{
	WSACleanup();
}

////////////private////////////

SOCKET TCPListener::createSoctet()
{
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening != INVALID_SOCKET) {
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(_port);
		inet_pton(AF_INET, _ipAddress.c_str(), &hint.sin_addr);

		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != SOCKET_ERROR) {
			int listenOk = listen(listening, SOMAXCONN);
			if (listenOk == SOCKET_ERROR) {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	return listening;
}

SOCKET TCPListener::waitForConnection(SOCKET listening)
{
	SOCKET client = accept(listening, nullptr, nullptr);
	return client; 
}
