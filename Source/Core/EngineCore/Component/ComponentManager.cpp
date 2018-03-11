#include "ComponentManager.h"

namespace cube
{
	namespace core
	{
		ComponentManager::ComponentManager()
		{
		}

		ComponentManager::~ComponentManager()
		{
		}

		SPtr<Component> ComponentManager::CreateComponent(const String& name)
		{
			auto findIter = mComponentCreators.find(name);
			if(findIter == mComponentCreators.end()) {
				CUBE_LOG(LogType::Error, fmt::format(L"Cannot create component \"{0}\". It hasn't been registerd"));
				return nullptr;
			}

			return findIter->second();
		}
	}
}