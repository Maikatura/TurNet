#pragma once
#include <atomic>
#include <functional>
#include <vector>


namespace TurNet
{
	class WorkerThread
	{
		std::atomic_bool done;
		std::vector<std::function<void()>> workQueue;

	public:
		


	};
	
}

