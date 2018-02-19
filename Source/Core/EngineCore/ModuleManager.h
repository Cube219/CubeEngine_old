#pragma once

#include "EngineCoreHeader.h"

#include "BaseModule/BaseModule.h"
#include "BasePlatform/BasePlatform.h"
#include "Thread/ThreadManager.h"
#include "Thread/Thread.h"

namespace cube
{
	namespace core
	{
		class ModuleManager
		{
		public:
			ModuleManager(SPtr<ThreadManager>& threadManager);
			~ModuleManager();

			void AddModule(SPtr<module::BaseModule>& m);
			
		private:
			void RunModule(SPtr<module::BaseModule>& module);

			HashMap<const char*, SPtr<module::BaseModule>> mModuleLookup;

			Vector<SPtr<Thread>> mThreadsToRunModules;

			SPtr<ThreadManager> mThreadManager;
		};
	}
}