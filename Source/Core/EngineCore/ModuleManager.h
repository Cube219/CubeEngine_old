#pragma once

#include "EngineCoreHeader.h"

#include <functional>

#include "BaseModule/BaseModule.h"
#include "BasePlatform/BasePlatform.h"
#include "Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		// TODO: dependency에 따라서 Module들 병렬로 처리
		//       지금은 그냥 넣은 순서대로 처리중
		struct ModuleNode
		{
			enum class State
			{
				NotRun, Running, Finished
			};

			SPtr<platform::BasePlatformDLib> moduleDLib;
			SPtr<module::BaseModule> module;
			uint32_t remainDependencyNum;
			State state;
		};

		class ENGINE_CORE_EXPORT ModuleManager
		{
		public:
			ModuleManager(SPtr<platform::BasePlatform>& platform, SPtr<ThreadManager>& threadManager);
			~ModuleManager();

			void LoadModule(String moduleName);

			void InitModules();

			SPtr<module::BaseModule> GetModule(String& name);
			
			template <typename T>
			SPtr<T> GetModule(String& name)
			{
				return DPCast(T)(GetModule(name));
			}

			void UpdateAllModules(float dt);

		private:
			HashMap<String, SPtr<module::BaseModule>> mModuleLookup;

			Vector<ModuleNode> mModules;

			SPtr<platform::BasePlatform> mPlatform;
			SPtr<ThreadManager> mThreadManager;
		};
	}
}