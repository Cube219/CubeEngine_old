#include "ModuleManager.h"

#include "Thread/ThreadManager.h"
#include "Thread/Thread.h"

#include "LogWriter.h"

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
			const char* moduleName = m->GetModuleInfo().name;

			auto temp = mModuleLookup.find(moduleName);
			if(temp == mModuleLookup.end()) {
				LogWriter::WriteLog(fmt::format(L"ModuleManaer: Already module name \"{0}\" loaded.", moduleName));
				return;
			}

			mModuleLookup[moduleName] = m;

			Vector<const char*>& moduleDependencies = m->GetModuleInfo().dependencies;
		}

		void ModuleManager::RunModule(SPtr<module::BaseModule>& module)
		{
		}
	}
}