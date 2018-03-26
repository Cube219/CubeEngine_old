#pragma once

#include "EngineCoreHeader.h"

#include <functional>

#include "DLib.h"
#include "BaseModule/BaseModule.h"
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

			SPtr<platform::DLib> moduleDLib;
			SPtr<module::BaseModule> module;
			uint32_t remainDependencyNum;
			State state;
		};

		class ENGINE_CORE_EXPORT ModuleManager
		{
		public:
			ModuleManager(SPtr<ThreadManager>& threadManager);
			~ModuleManager();

			void LoadModule(String2 moduleName);

			void InitModules();

			SPtr<module::BaseModule> GetModule(String2& name);
			
			template <typename T>
			SPtr<T> GetModule(String2& name)
			{
				return DPCast(T)(GetModule(name));
			}

			void UpdateAllModules(float dt);

		private:
			HashMap<String2, SPtr<module::BaseModule>> mModuleLookup;

			Vector<ModuleNode> mModules;

			SPtr<ThreadManager> mThreadManager;
		};
	}
}