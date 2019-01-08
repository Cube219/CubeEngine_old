#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		struct ModuleInfo
		{
			const char* name;
			Vector<const char*> dependencies;
		};

		class ENGINE_CORE_EXPORT BaseModule
		{
		public:
			BaseModule() {}
			virtual ~BaseModule() {}

			ModuleInfo GetModuleInfo() const { return mInfo; }

			virtual void Init(core::EngineCore* eCore, ...) = 0;

			virtual void Update(float dt) = 0;

			virtual void Destroy() = 0;

		protected:
			ModuleInfo mInfo;
		};
	} // namespace core
} // namespace cube
