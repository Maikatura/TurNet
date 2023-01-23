#pragma once
#include <WS2tcpip.h>

namespace TurNet
{

	bool CompareClients(sockaddr_in& aClientOne, sockaddr_in& aClientTwo)
	{
		int sizeOne = sizeof(aClientOne);
		int sizeTwo = sizeof(aClientTwo);
		if ((sizeOne == sizeTwo) && (memcmp(&aClientOne, &aClientTwo, sizeTwo) == 0))
		{
			return true;
		}

		return false;
	}

	
}
