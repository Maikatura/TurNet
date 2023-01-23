#include "ServerUDP.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <WS2tcpip.h>

#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/MessageData.h"

bool TurNet::ServerUDP::Start()
{
	struct sockaddr_in server{};

	if(!StartWinSock())
	{
		return false;
	}

	if((myListenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "socket() failed with error code : " << WSAGetLastError() << std::endl;;
		closesocket(myListenSocket);
		WSACleanup();
		return false;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(DEFAULT_PORT);

	myReturnResult = bind(myListenSocket, (struct sockaddr*)&server, sizeof(server));
	if(myReturnResult == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		closesocket(myListenSocket);
		WSACleanup();
		return false;
	}

	std::cout << "Winsock started up without problems!\n";
	return true;
}

void TurNet::ServerUDP::StartWorker()
{
	myWorkerShouldRun = true;
	myWorkerThread = std::thread([this]() { WorkerThread(); });
}

int TurNet::ServerUDP::SendToClient(sockaddr_in aAddress, TurNet::TurMessage& aMessage)
{
	std::string messageToClient = aMessage.BufferRaw();

	if(messageToClient.size() > DEFAULT_BUFFER_SIZE)
	{
		std::cout << "Can't send a message over the size of: " << DEFAULT_BUFFER_SIZE << "\n";
		return 0;
	}

	char aDataBuffer[DEFAULT_BUFFER_SIZE];
	std::memcpy(aDataBuffer, messageToClient.c_str(), messageToClient.length());

	return SendToClientRaw(aAddress, aDataBuffer, static_cast<int>(messageToClient.size()));
}

std::vector<sockaddr_in>& TurNet::ServerUDP::GetClients()
{
	return myClients;
}

int TurNet::ServerUDP::SendToClientRaw(sockaddr_in aAddress, char aDataBuffer[512], int aSize)
{
	int length = sizeof(struct sockaddr_in);
	int result = sendto(myListenSocket, aDataBuffer, aSize, 0, (const struct sockaddr*)&aAddress, length);
	if(result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(myListenSocket);
		WSACleanup();
		myWorkerShouldRun = false;
	}

	return result;
}

void TurNet::ServerUDP::WorkerThread()
{

	while(myWorkerShouldRun)
	{
		MessageData messageData;
		memset(messageData.Buffer, 0, 512);

		int slen = sizeof(messageData.Connection);
		if((messageData.BufferReceiveSize = recvfrom(myListenSocket, messageData.Buffer, messageData.BufferLength, 0, (struct sockaddr*)&messageData.Connection, &slen)) == SOCKET_ERROR)
		{
			std::cout << "recvfrom() failed with error code : " << WSAGetLastError();
		}

#pragma warning(suppress : 4996)
		const char* ipAddress = inet_ntoa(messageData.Connection.sin_addr);
		std::cout << "Packet from " << ipAddress << ":" << messageData.Connection.sin_port << "\n";
		std::cout << "Packet Size: " << messageData.BufferReceiveSize << "\n";


		TurNet::TurMessage data;
		TurNet::Convertor(data, messageData);

		data.Header.Connection = messageData.Connection;

		TurNet::NetworkDataTypes type = data.Header.ID;
		switch(type)
		{
			case TurNet::NetworkDataTypes::Connect:
			{
				TurNet::TurMessage message;
				message.Header.ID = NetworkDataTypes::Connect;
				std::string connectMessage = "Welcome to this server!";

				message << connectMessage;
				SendToClient(messageData.Connection, message);

				{
					std::scoped_lock<std::mutex> lock(myThreadLock);
					myClients.push_back(messageData.Connection);
				}
			
				break;
			}
			case TurNet::NetworkDataTypes::Message:
			{
				myMessageLoop(data);
				break;
			}
		}
		
		

	}

	myReturnResult = shutdown(myListenSocket, SD_SEND);
	if(myReturnResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(myListenSocket);
		WSACleanup();
	}

	myWorkerThread.join();
}
