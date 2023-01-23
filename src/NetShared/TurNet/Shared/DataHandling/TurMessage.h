#pragma once
#include <cstdint>
#include <ostream>
#include <vector>
#include "MessageData.h"

namespace TurNet
{
	template<typename T>
	struct TurMessageHeader
	{
		T ID{};
		uint32_t Size = 0;
		sockaddr_in Connection;
	};


	struct TurMessage
	{
		TurMessageHeader<NetworkDataTypes> Header{};
		std::vector<char> Body;

		TurMessage() { Header.ID = NetworkDataTypes::Message; }
		TurMessage(const TurMessage& someData) = default;
		~TurMessage() = default;

		size_t size()
		{
			return Body.size();
		}

		friend std::ostream& operator<<(std::ostream& aOutstream, const TurMessage& aMessage)
		{
			aOutstream << "ID: " << char(aMessage.Header.ID) << "Size:" << aMessage.Header.Size;
			return aOutstream;
		}

		template<typename DataType>
		friend TurMessage& operator<<(TurMessage& msg, const DataType& someData)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = msg.Body.size();

			msg.Body.resize(msg.Body.size() + sizeof(DataType));

			std::memcpy(msg.Body.data() + i, &someData, sizeof(DataType));

			msg.Header.Size = static_cast<int>(msg.size());

			return msg;
		}

		template<typename DataType>
		friend TurMessage& operator>>(TurMessage& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");
			size_t i = msg.size() - sizeof(DataType);

			std::memcpy(&data, msg.Body.data() + i, sizeof(DataType));

			msg.Body.resize(i);
			msg.Header.Size = static_cast<int>(msg.size());
			return msg;
		}

		
		friend TurMessage& operator<<(TurMessage& msg, const std::string& someData)
		{
			size_t i = msg.Body.size();
			msg.Body.resize(msg.Body.size() + someData.size());

			std::memcpy(msg.Body.data() + i, someData.c_str(), someData.size());
			msg.Header.Size = static_cast<int>(msg.size());

			msg << static_cast<short>(someData.size());

			return msg;
		}

		friend TurMessage& operator>>(TurMessage& msg, std::string& data)
		{
			short stringSize = 0;
			msg >> stringSize;

			data.resize(stringSize);

			size_t i = msg.size() - stringSize;

			std::memcpy((void*)data.data(), msg.Body.data() + i, stringSize);

			msg.Body.resize(i);

			msg.Header.Size = static_cast<int>(msg.size());

			return msg;
		}


		std::string BufferRaw()
		{
			std::string returnString;
			returnString = (char)Header.ID;

			char size[4];
			std::memcpy(size, &Header.Size, 4);

			for(size_t i = 0; i < 4; i++)
			{
				returnString += size[i];
			}

			for(size_t i = 0; i < Body.size(); i++)
			{
				returnString += Body[i];
			}

			return returnString;
		}
	};

	inline TurMessage& Convertor(TurMessage& aNewMessage, const MessageData& someData)
	{
		std::memcpy(&aNewMessage.Header.ID, &someData.Buffer[0], 1);
		std::memcpy(&aNewMessage.Header.Size, someData.Buffer + 1, 4);

		size_t size = aNewMessage.Header.Size;
		aNewMessage.Body.resize(size);

		std::memcpy(aNewMessage.Body.data(), someData.Buffer + 5, size);

		return aNewMessage;
	}
}
