#pragma once
#include <string>

namespace TurNet
{

	class ClientTCP
	{
	public:
		ClientTCP();
		ClientTCP(std::string aHostName, int aPort);
		~ClientTCP();
	private:

	};

}