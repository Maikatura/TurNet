#pragma once
#include <map>

class MessageData
{
public:

	template<typename T>
	void operator<<(T aData);

	template<typename T>
	T operator>>(T aVariable);

private:

	std::map<size_t, size_t> myBuffer;
};

template <typename T>
void MessageData::operator<<(T aData)
{
	myBuffer.insert(aData);
}
