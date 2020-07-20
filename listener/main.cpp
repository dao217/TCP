#include<iostream>
#include<string>
#include"TcpListener.h"

void Listener_MessageReceived(TCPListener* listener, int client, std::string msg) {
	listener->sendMessage(client, msg);
}

int main() {

	TCPListener server("127.0.0.1", 54010, Listener_MessageReceived);

	if (server.init()) {
		server.run();
	}

	return 0;
}