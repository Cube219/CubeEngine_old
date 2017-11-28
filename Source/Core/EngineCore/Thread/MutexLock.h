#pragma once

#include <mutex>
#include <atomic>

namespace cube
{
	namespace core
	{
		using Mutex = std::mutex;
		using Lock = std::unique_lock<Mutex>;
		template <typename T>
		using Atomic = std::atomic<T>;

		using ThreadNotify = std::condition_variable;
	}
}