#pragma once

#include <mutex>

namespace cube
{
	namespace core
	{
		using Mutex = std::mutex;
		using Lock = std::unique_lock<Mutex>;
		using ThreadNotify = std::condition_variable;
	}
}