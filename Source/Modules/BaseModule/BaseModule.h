#pragma once
#include "Base/BaseTypes.h"

namespace cube
{
	namespace module
	{
		struct ModuleInfo
		{
			const char* name;
			bool isRunAfterComponent;
			Vector<const char*> dependencies;
		};

		class BaseModule
		{
		public:
			BaseModule(){ }
			virtual ~BaseModule(){ }

			ModuleInfo GetModuleInfo() const { return mInfo; }

			virtual void Init() = 0;

			virtual void Update(float dt) = 0;

			virtual void Destroy() = 0;

		protected:
			ModuleInfo mInfo;
		};
	}
}