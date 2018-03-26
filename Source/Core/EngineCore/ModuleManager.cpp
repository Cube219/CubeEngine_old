#include "ModuleManager.h"

#include "Platform.h"

#include "Thread/ThreadManager.h"
#include "Thread/Thread.h"

#include "Base/format.h"
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
			for(auto& m : mModules) {
				m.module->Destroy();
			}

			// mModuleLoopup must be freed before mModules
			// Because the module cannot be freed after its DLib has been unloaded
			mModuleLookup.clear();
			mModules.clear();
		}

		void ModuleManager::LoadModule(String2 moduleName)
		{
			auto temp = mModuleLookup.find(moduleName);
			if(temp != mModuleLookup.end()) {
				CUBE_LOG(LogType::Error, "Already module name \"{0}\" loaded.", moduleName);
				return;
			}

			ModuleNode node;
			
			node.moduleDLib = platform::Platform::LoadDLib(moduleName);
			
			using CreateModuleFunction = module::BaseModule* (*)();

			auto createModuleFunction = RCast(CreateModuleFunction)(node.moduleDLib->GetFunction(CUBE_T("CreateModule")));
			node.module = SPtr<module::BaseModule>(createModuleFunction());

			mModules.push_back(node);
			mModuleLookup[moduleName] = node.module;
		}

		void ModuleManager::InitModules()
		{
			for(auto& m : mModules) {
				m.module->Init(ECore());
			}
		}

		SPtr<module::BaseModule> ModuleManager::GetModule(String2& name)
		{
			auto temp = mModuleLookup.find(name.c_str());
			if(temp == mModuleLookup.end()) {
				CUBE_LOG(LogType::Error, "Cannot find module \"{0}\".", name);
				return nullptr;
			}

			return temp->second;
		}

		void ModuleManager::UpdateAllModules(float dt)
		{
			for(auto iter = mModules.begin(); iter != mModules.end(); iter++) {
				iter->module->Update(dt);
			}
		}
	}
}