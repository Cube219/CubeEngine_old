#pragma once

#include "../EngineCoreHeader.h"

#include <functional>

#include "DLib.h"
#include "../Thread/MutexLock.h"

namespace cube
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
		SPtr<BaseModule> module;
		Uint32 remainDependencyNum;
		State state;
	};

	class ENGINE_CORE_EXPORT ModuleManager
	{
	public:
		ModuleManager() {}
		~ModuleManager() {}

		ModuleManager(const ModuleManager& other) = delete;
		ModuleManager& operator=(const ModuleManager& rhs) = delete;
		ModuleManager(ModuleManager&& other) = delete;
		ModuleManager& operator=(ModuleManager&& rhs) = delete;

		void Initialize();
		void ShutDown();

		void LoadModule(StringRef moduleName);

		void InitModules();

		SPtr<BaseModule> GetModule(StringRef name);
			
		template <typename T>
		SPtr<T> GetModule(StringRef name)
		{
			return DPCast(T)(GetModule(name));
		}

		void UpdateAllModules(float dt);

	private:
		HashMap<String, SPtr<BaseModule>> mModuleLookup;

		Vector<ModuleNode> mModules;
	};
} // namespace cube
