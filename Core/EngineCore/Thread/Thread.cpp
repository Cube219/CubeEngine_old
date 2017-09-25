#include "Thread.h"

namespace cube
{
	namespace core
	{
		Thread::Thread()
		{
		}

		Thread::~Thread()
		{
		}

		void Thread::Start(std::function<void()> function)
		{
			mThread = std::thread(function);
		}

		void Thread::WaitUntilFinished()
		{
			mThread.join();
		}
	}
}