#pragma once
#include <iostream>
#include <string>


namespace TurNet
{
	enum class NetworkType : char
	{
		None = 0,
		Connect = 1,
		Message,
		EXIT,
		Count
	};


	//class NetMessageBase
	//{
	//protected:

	//	void Reload()
	//	{
	//		for(int i = 0; i < DEFAULT_MESSAGE_BUFFER_SIZE; i++)
	//		{
	//			myId[i] = (char)255;
	//		}
	//	}

	//	template<typename T>
	//	char ID(T aType) { return static_cast<char>(aType); }
	//public:
	//	char myId[DEFAULT_MESSAGE_BUFFER_SIZE];
	//};

	//class ConnectMessage : public NetMessageBase
	//{
	//public:
	//	ConnectMessage()
	//	{
	//		myId[0] = ID(NetworkType::Connect);
	//		strcpy_s(myMessage, "Welcome to the server");
	//	}
	//	char myMessage[500];
	//};

	//class PlainMessage : public NetMessageBase
	//{
	//	const static int MaxMessageSize = 100;

	//	char myMessage[MaxMessageSize];

	//public:
	//	PlainMessage() : NetMessageBase()
	//	{
	//		Reload();
	//		myId[0] = ID(NetworkType::Message);
	//		myId[1] = ID(NetworkDataTypes::Text);
	//	}

	//	bool SetData(std::BufferRaw aMessage)
	//	{
	//		if(aMessage.size() > MaxMessageSize)
	//		{
	//			std::cout << "A message cant be bigger than 500 chars\n";
	//			return false;
	//		}
	//		
	//		strcpy_s(myMessage, aMessage.c_str());
	//		myMessage[sizeof(myMessage) - 1] = 0;

	//		return true;
	//	}

	//	std::BufferRaw BufferRaw()
	//	{
	//		std::BufferRaw returnString;
	//		for(size_t i = 0; i < MaxMessageSize; i++)
	//		{
	//			if(((myMessage[i] >= 31) && (myMessage[i] <= 122)))
	//			{
	//				returnString += myMessage[i];
	//			}
	//		}

	//		return returnString;
	//	}

	//};

	
}
