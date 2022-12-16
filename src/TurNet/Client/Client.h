#pragma once
#include <iostream>
#include <winsock2.h>

class Client
{
public:
	static void Start()
	{
		// Initialize Winsock
		WSADATA wsaData;
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if(result != 0)
		{
			std::cerr << "Error initializing Winsock" << std::endl;

		}

		// Create a socket
		SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sockfd == INVALID_SOCKET)
		{
			std::cerr << "Error creating socket" << std::endl;
		}

		// Connect to the server
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		// ReSharper disable once CppDeprecatedEntity

#pragma warning( disable : 4996 )
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(8000);
		if(connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			std::cerr << "Error connecting to server" << std::endl;
		}
#pragma warning( pop )

		// Send data to the server
		const char* message = "Hello from the client!";
		int num_bytes = send(sockfd, message, strlen(message), 0);
		if(num_bytes == SOCKET_ERROR)
		{
			std::cerr << "Error sending data to server" << std::endl;
		}

		// Close the socket
		closesocket(sockfd);

		// Clean up Winsock
		WSACleanup();

	}
};