#include "ModuleManager.h"

#include "Thread/ThreadManager.h"
#include "Thread/Thread.h"

#include "Base/format.h"
#include "LogWriter.h"

namespace cube
{
	namespace core
	{
		ModuleManager::ModuleManager(SPtr<platform::BasePlatform>& platform, SPtr<ThreadManager>& threadManager) :
			mPlatform(platform), mThreadManager(threadManager)
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

		void ModuleManager::LoadModule(String moduleName)
		{
			auto temp = mModuleLookup.find(moduleName);
			if(temp != mModuleLookup.end()) {
				CUBE_LOG(LogType::Error, fmt::format(L"Already module name \"{0}\" loaded.", moduleName));
				return;
			}

			ModuleNode node;
			
			WString moduleWName;
			// !위 변환은 String이 ACSII인 경우만 가능
			// 따라서 다른 곳에서는 이 방식을 쓰면 안 됨. 차후에 String 시스템 개선 때 고쳐질 것
			moduleWName.assign(moduleName.begin(), moduleName.end());
			node.moduleDLib = mPlatform->LoadDLib(moduleWName);
			
			using CreateModuleFunction = module::BaseModule* (*)();

			auto createModuleFunction = RCast(CreateModuleFunction)(node.moduleDLib->GetFunction("CreateModule"));
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

		SPtr<module::BaseModule> ModuleManager::GetModule(String& name)
		{
			auto temp = mModuleLookup.find(name.c_str());
			if(temp == mModuleLookup.end()) {
				CUBE_LOG(LogType::Error, fmt::format(L"Cannot find module \"{0}\".", name));
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