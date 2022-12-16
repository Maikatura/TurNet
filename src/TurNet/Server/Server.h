#pragma once

#include <iostream>
#include <winsock2.h>

class Server
{
    static void receive_message(int sockfd)
    {
        //// Read the size of the message
        //uint32_t message_size;
        //int result = read(sockfd, &message_size, sizeof(message_size));
        //if(result < 0)
        //{
        //    std::cerr << "Error reading message size: " << std::strerror(errno) << std::endl;
        //    return;
        //}

        //// Convert the message size from network byte order to host byte order
        //message_size = ntohl(message_size);

        //// Read the message data
        //std::string message(message_size, '\0');
        //result = read(sockfd, &message[0], message_size);
        //if(result < 0)
        //{
        //    std::cerr << "Error reading message data: " << std::strerror(errno) << std::endl;
        //    return;
        //}

        //// Print the message
        //std::cout << "Message received: " << message << std::endl;
    }

public:
	static void Start()
	{
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

        // Bind the socket to a local address and port
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(8000);
        if(bind(sockfd, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
        {
            std::cerr << "Error binding socket to address" << std::endl;
        }

        // Listen for incoming connections
        if(listen(sockfd, 5) == SOCKET_ERROR)
        {
            std::cerr << "Error listening for incoming connections" << std::endl;
        }

        // Accept an incoming connection
        sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        SOCKET client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
        if(client_sockfd == INVALID_SOCKET)
        {
            std::cerr << "Error accepting connection" << std::endl;
        }

        // Communicate with the client
        char buffer[1024];
        while(true)
        {
            // Receive data from the client
            int num_bytes = recv(client_sockfd, buffer, sizeof(buffer), 0);
            if(num_bytes == SOCKET_ERROR || num_bytes == 0)
            {
                break;
            }

            // Print the received data to the console
            std::cout << "Received: " << buffer << std::endl;

            // Send a response back to the client
            send(client_sockfd, "Hello from the server!", sizeof("Hello from the server!"), 0);
        }

        // Close the socket
        closesocket(sockfd);

        // Clean up Winsock
        WSACleanup();
	}
};