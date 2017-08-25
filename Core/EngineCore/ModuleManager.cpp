#include "ModuleManager.h"

namespace cube
{
	namespace core
	{
		ModuleManager::ModuleManager(SPtr<platform::BasePlatform>& platform) : 
			mPlatform_ref(platform)
		{
		}

		ModuleManager::~ModuleManager()
		{
		}
	}
}