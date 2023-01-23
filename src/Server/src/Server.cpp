#include <iostream>
#include "TurNet/Server/ServerUDP.h"

#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/TurMessage.h"
#include "DontUse/DontUseThisThisIsJustForTesting.h"
#include "TurNet/Shared/Utility/TurCompare.h"

int main()
{
    TurNet::ServerUDP server;
    if(!server.Start())
    {
        std::cout << "Failed to start socket\n";
    }

	std::function<void(TurNet::TurMessage&)> messageLoop = [&server](TurNet::TurMessage& someData)
	{
		
		char typeBase;
		someData >> typeBase;
		DontUseIt type = static_cast<DontUseIt>(typeBase);

		switch (type)
		{
		case DontUseIt::ClientMessage:
			std::string message = "";

			someData >> message;
			std::cout << "Data: " << message << "\n";

			std::vector<sockaddr_in>& clients = server.GetClients();
			for (int i = 0; i < clients.size(); i++)
			{
				if (!TurNet::CompareClients(someData.Header.Connection, clients[i]))
				{
					TurNet::TurMessage messageOut;

					auto messageType = static_cast<char>(DontUseIt::ClientMessage);
					messageOut << message;
					messageOut << messageType;

					server.SendToClient(clients[i], messageOut);
				}
			}

			break;
		}
	};

	server.SetReceiveMessageLoop(messageLoop);
    server.StartWorker();

	bool hasPrinted = false;

	while (true)
	{
		if (!hasPrinted)
		{
			std::cout << "Server is started and this is the update loop :)\n";
			hasPrinted = true;
		}
	}


    system("PAUSE");
}
