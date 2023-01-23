#include <iostream>
#include "TurNet/Client/ClientUDP.h"
#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/TurMessage.h"
#include "DontUse/DontUseThisThisIsJustForTesting.h"

void GameLoop(TurNet::ClientUDP& aClient)
{
	std::string temp;
	std::getline(std::cin, temp);
	std::this_thread::sleep_for(std::chrono::duration<float>(1.0f));

	bool shouldRunGameLoop = true;
	while(shouldRunGameLoop)
	{
		std::this_thread::sleep_for(std::chrono::duration<float>(0.1f));
		std::cout << "Enter Message: ";
		std::string message;

		if (message == "exit")
		{
			shouldRunGameLoop = false;
			return;
		}

		std::cin.clear();
		std::getline(std::cin, message);


		TurNet::TurMessage messageOut;

		auto messageType = static_cast<char>(DontUseIt::ClientMessage);
		messageOut << message;
		messageOut << messageType;

		aClient.SendToServer(messageOut);

	}
}

int main()
{

	std::cout << sizeof(int) << "\n";

	std::string serverIp;
	int serverPort;

	std::cout << "Type: 0 to auto complete\n";

	std::cout << "Server IP:";
	std::cin >> serverIp;

	if(serverIp == "0")
	{
		serverIp = "127.0.0.1";
	}

	std::cout << "Server IP:";
	std::cin >> serverPort;
	if(serverPort == 0)
	{
		serverPort = DEFAULT_PORT;
	}


    // All ClientUDP Network Code!

    TurNet::ClientUDP client;
	client.Connect(serverIp, serverPort);

    if (!client.Start())
    {
        std::cout << "Failed to create socket" << std::endl;
    }

	char myDataBuffer[DEFAULT_BUFFER_SIZE];

	/*TurNet::ConnectMessage connectMessage;
	memcpy(myDataBuffer, &connectMessage, sizeof(TurNet::ConnectMessage));
	client.SendToServerRaw(myDataBuffer);*/
	std::cout << sizeof(TurNet::TurMessageHeader< TurNet::NetworkDataTypes>) << "\n";
	memset(myDataBuffer, 0, DEFAULT_BUFFER_SIZE);

	TurNet::TurMessage connectMessage;
	connectMessage.Header.ID = TurNet::NetworkDataTypes::Connect;

	client.SendToServer(connectMessage);


	std::function<void(TurNet::TurMessage&)> messageLoop = [](TurNet::TurMessage& someData)
	{
		char typeBase;
		someData >> typeBase;
		DontUseIt type = static_cast<DontUseIt>(typeBase);

		switch(type)
		{
			case DontUseIt::ClientMessage:
				std::string message = "";

				someData >> message;
				std::cout << "\nSomeone typed: " << message << "\n";

				break;
		}
	};

	client.SetReceiveMessageLoop(messageLoop);
	client.StartWorker();

	GameLoop(client);
	system("PAUSE");
}
