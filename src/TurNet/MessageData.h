#pragma once
#include <map>
#include <iostream>
#include <vector>

namespace TurNet
{
	
	template <typename T>
	struct MessageHeader
	{
		T id {};
		uint32_t size = 0;
	};

	template <typename T>
	class MessageData
	{
		MessageHeader<T> header {};
		std::vector<uint8_t> body;

		[[nodiscard("You don't do anything with the size!")]] size_t size() const
		{
			return body.size();
		}

		friend std::ostream& operator << (std::ostream& os, const MessageData<T>& msg)
		{
			os << "ID:" << static_cast<int>(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		template<typename DataType>
		friend MessageData<T>& operator<<(MessageData<T>& msg, const DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
			size_t i = msg.body.size();
			msg.body.resize(msg.body.size() + sizeof(DataType));
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
			msg.header.size = msg.size();

			return msg;
		}

		template<typename DataType>
		friend MessageData<T>& operator>>(MessageData<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");
			size_t i = msg.body.size() - sizeof(DataType);
			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
			msg.body.resize(i);
			msg.header.size = msg.size();

			return msg;
		}
	};


	

}
