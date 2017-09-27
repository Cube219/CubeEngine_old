#pragma once

#include "EngineCoreHeader.h"

#include "BaseModule\BaseModule.h"
#include "BasePlatform\BasePlatform.h"

namespace cube
{
	namespace core
	{
		class ModuleManager
		{
		public:
			ModuleManager(SPtr<platform::BasePlatform>& platform);
			~ModuleManager();
			
		private:
			SPtr<platform::BasePlatform> mPlatform_ref;
		};
	}
}