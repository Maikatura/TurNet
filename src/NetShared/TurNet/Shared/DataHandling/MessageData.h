#pragma once
#include <vector>
#include <WS2tcpip.h>

#include "NetworkDataTypes.h"

namespace TurNet
{
	#define DEFAULT_BUFFER_SIZE 512

	/**
	 * \ THIS WILL BE REMOVE WHEN MY NEW HEADER SYSTEM IS IMPLEMENTED
	 */
	
	class MessageData
	{
		int CurrentDataPosition = 0;

	public:
		sockaddr_in Connection;
		int BufferReceiveSize;
		char Buffer[DEFAULT_BUFFER_SIZE]{};
		int BufferLength = DEFAULT_BUFFER_SIZE;


		// This method take the memory in order use it at your own risk
		template<class T>
		T Get(int aCustomSize = -1)
		{
			T output{};

			int size;
			if (aCustomSize == -1)
			{
				size = sizeof(T);
			}
			else
			{
				size = aCustomSize;
			}
			std::vector<char> bufferCopy;

			int index = 0;
			for (int i = CurrentDataPosition; i < BufferLength; i++)
			{
				if (index < size)
				{
					bufferCopy.push_back(Buffer[i]);
					index++;
				}
				else
				{
					break;
				}
			}

			memcpy(&output, bufferCopy.data(), sizeof(T));
			CurrentDataPosition += size;

			return output;
		}

		template<class T>
		T Convert()
		{
			T output{};
			memcpy(&output, Buffer, sizeof(T));
			return output;
		}
	};
	
}


