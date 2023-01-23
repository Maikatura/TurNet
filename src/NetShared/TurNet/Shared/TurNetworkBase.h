#pragma once
#define DEFAULT_PORT 5000
#include <functional>
#include <mutex>
#include <thread>

#include "DataHandling/MessageData.h"
#include "DataHandling/TurMessage.h"

namespace TurNet
{

	class TurNetworkBase
	{
	protected:
		int myReturnResult = 0;
		std::atomic_bool myWorkerShouldRun;

		std::function<void(TurMessage&)> myMessageLoop;

	public:
		virtual bool Start();
		virtual void StartWorker();

		void SetReceiveMessageLoop(std::function<void(TurMessage&)>& aMessageLoop);

	protected:
		bool StartWinSock();
		virtual void WorkerThread() = 0;

		std::mutex myThreadLock;
		std::thread myWorkerThread;
	
	};
}

