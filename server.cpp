#include<iostream>
#include<WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

int main() {

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsok = WSAStartup(ver, &wsData);

	if (wsok != 0) {
		std::cerr << "Can't start Winsock (err " << wsok << std::endl;
		return -1;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Can't create socket, (err " <<
			WSAGetLastError() << ") " << std::endl;
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Can't create socket, (err " <<
			WSAGetLastError() << ") " << std::endl;
		return -1;
	}
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << "connected on port" << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "connected on port" <<
			htons(client.sin_port) << std::endl;
	}

	closesocket(listening);
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);
		int byteReceived = recv(clientSocket, buf, 4096, 0);
		if (byteReceived == SOCKET_ERROR) {
			std::cerr << "some error in recv()" << std::endl;
			break;
		}

		if (byteReceived == 0)
		{
			std::cout << "client disconencted" << std::endl;
			break;
		}
		std::cout<< "client> "<< buf << std::endl;
		send(clientSocket, buf, byteReceived + 1, 0);
	}
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}