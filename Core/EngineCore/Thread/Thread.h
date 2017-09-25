#pragma once

#include "../EngineCoreHeader.h"

#include <thread>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Thread
		{
		public:
			Thread();
			~Thread();

			void Start(std::function<void()> function);

			void WaitUntilFinished();

		private:
			std::thread mThread;
		};
	}
}