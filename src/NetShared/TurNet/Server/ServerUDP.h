#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <WS2tcpip.h>
#include "TurNet/Shared/TurNetworkBase.h"

namespace TurNet
{
	class ServerUDP : public TurNetworkBase
	{
	public:
		bool Start() override;

		void StartWorker() override;
		int SendToClient(sockaddr_in aAddress, TurNet::TurMessage& aMessage);

		std::vector<sockaddr_in>& GetClients();

	private:

		int SendToClientRaw(sockaddr_in aAddress, char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize);
		void WorkerThread() override;

		int myCurrentClientId = 0;

		SOCKET myListenSocket = INVALID_SOCKET;


		struct addrinfo* myResult = NULL;
		struct addrinfo* myHints = NULL;

		
		std::vector<sockaddr_in> myClients;
		
	};
		
}


