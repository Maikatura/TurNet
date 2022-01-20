#include "pch.h"
#include "Client.h"

#include <cassert>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

TurNet::ClientTCP::ClientTCP()
{
	// Init Websocket
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		assert(wsOk != 0 && "Can't init winsock");
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if(listening == INVALID_SOCKET)
	{
		assert(listening != INVALID_SOCKET && "Can't create a socket!");
		return;
	}

	// Bind an ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family	= AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

	bind(listening, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));

	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, reinterpret_cast<sockaddr*>(&client), &clientSize);

	if (clientSocket == INVALID_SOCKET)
	{
		
	}

	char host[NI_MAXHOST];		// Client's Remote name
	char service[NI_MAXHOST];	// Service (i.e. port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);



	// Close Listening socket


	// While loop: accept and echo message back to client


}
