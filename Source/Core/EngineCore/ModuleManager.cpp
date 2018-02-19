#include "ModuleManager.h"

namespace cube
{
	namespace core
	{
		ModuleManager::ModuleManager(SPtr<ThreadManager>& threadManager) :
			mThreadManager(threadManager)
		{
		}

		ModuleManager::~ModuleManager()
		{
		}

		void ModuleManager::AddModule(SPtr<module::BaseModule>& m)
		{
		}

		void ModuleManager::RunModule(SPtr<module::BaseModule>& module)
		{
		}
	}
}