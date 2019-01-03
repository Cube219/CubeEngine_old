#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	namespace core
	{
		class EngineCore;
	}
}

namespace cube
{
	namespace module
	{
		struct ModuleInfo
		{
			const char* name;
			Vector<const char*> dependencies;
		};

		class BaseModule
		{
		public:
			BaseModule(){ }
			virtual ~BaseModule(){ }

			ModuleInfo GetModuleInfo() const { return mInfo; }

			virtual void Init(core::EngineCore* eCore, ...) = 0;

			virtual void Update(float dt) = 0;

			virtual void Destroy() = 0;

		protected:
			ModuleInfo mInfo;
		};
	}
}