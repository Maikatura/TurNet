#include "ClientUDP.h"

#include <cassert>
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"



TurNet::ClientUDP::ClientUDP(): myConnectSocket(0), myServer(), myServerPort(0)
{
}

bool TurNet::ClientUDP::Connect(const std::string& aAddress, u_short aPort)
{
	myServerIP = aAddress;
	myServerPort = aPort;

	memset((char*)&myServer, 0, sizeof(myServer));
	myServer.sin_family = AF_INET;
	myServer.sin_port = htons(myServerPort);
#pragma warning(suppress : 4996)
	myServer.sin_addr.S_un.S_addr = inet_addr(myServerIP.c_str());

	return true;
}

bool TurNet::ClientUDP::Start()
{
	if(!StartWinSock())
	{
		return false;
	}

	if((myConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		std::cout << "socket() failed with error code : " << WSAGetLastError() << std::endl;
		closesocket(myConnectSocket);
		WSACleanup();
		return false;
	}

	std::cout << "Winsock started up without problems!\n";
	return true;
}

void TurNet::ClientUDP::StartWorker()
{
	myWorkerShouldRun = true;
	myWorkerThread = std::thread([this]() { WorkerThread(); });
}

int TurNet::ClientUDP::SendToServer(TurNet::TurMessage& aMessage)
{
	const std::string messageToServer = aMessage.BufferRaw();

	if (messageToServer.size() > DEFAULT_BUFFER_SIZE)
	{
		std::cout << "Can't send a message over the size of: " << DEFAULT_BUFFER_SIZE << "\n";
		return 0;
	}

	char aDataBuffer[DEFAULT_BUFFER_SIZE];
	std::memcpy(aDataBuffer, messageToServer.c_str(), messageToServer.length());

	return SendToServerRaw(aDataBuffer, static_cast<int>(messageToServer.size()));
}

int TurNet::ClientUDP::SendToServerRaw(char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize)
{
	int length = sizeof(struct sockaddr_in);
	int result = sendto(myConnectSocket, aDataBuffer, aSize, 0, (const struct sockaddr*)&myServer, length);
	if(result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(myConnectSocket);
		WSACleanup();
		myWorkerShouldRun = false;
	}

	return result;
}

void TurNet::ClientUDP::WorkerThread()
{
	

	while(myWorkerShouldRun)
	{
		// This is where the logic for the recieve loop should go for the client
		MessageData messageData;
		std::memset(messageData.Buffer, 0, 512);

		int slen = sizeof(messageData.Connection);
		if((messageData.BufferReceiveSize = recvfrom(myConnectSocket, messageData.Buffer, messageData.BufferLength, 0, (struct sockaddr*)&messageData.Connection, &slen)) == SOCKET_ERROR)
		{
			
		}

#pragma warning(suppress : 4996)
		/*const char* ipAddress = inet_ntoa(si_other.sin_addr);
		std::cout << "Packet from " << ipAddress << ":" << si_other.sin_port << "\n";*/


		TurNet::NetworkDataTypes type = (TurNet::NetworkDataTypes)messageData.Buffer[0];
		switch(type)
		{
			case TurNet::NetworkDataTypes::Connect:
			{
				std::cout << "You are now connected to the server!" << "\n";
				break;
			}
			case TurNet::NetworkDataTypes::Message:
			{
				TurNet::TurMessage data;
				TurNet::Convertor(data, messageData);
				myMessageLoop(data);
				break;
			}
		}

		
		

		//myReturnResult = 0;
		//if(myReturnResult < 0)
		//{
		//	std::cout << "Connection closed\n";
		//	myWorkerShouldRun = false;
		//}
		//else
		//{
		//	/*std::cout << "recv failed with error:" << WSAGetLastError() <<"\n";
		//	myWorkerShouldRun = false;*/
		//}
	}

	myReturnResult = shutdown(myConnectSocket, SD_SEND);
	if(myReturnResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(myConnectSocket);
		WSACleanup();
	}

	myWorkerThread.join();
}
