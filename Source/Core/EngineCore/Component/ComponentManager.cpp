#include "ComponentManager.h"

#include "../LogWriter.h"
#include "Component.h"

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

		HComponent ComponentManager::CreateComponent(const String& name)
		{
			auto findIter = mComponentCreators.find(name);
			if(findIter == mComponentCreators.end()) {
				CUBE_LOG(LogType::Error, "Cannot create component \"{0}\". It hasn't been registerd", name);
				return HComponent();
			}

			return findIter->second();
		}

		void ComponentManager::CheckIfComponentExisted(const String& name)
		{
			auto findIter = mComponentCreators.find(name);
			if(findIter != mComponentCreators.end()) {
				CUBE_LOG(LogType::Error, "Component \"{0}\" is already registered", name);
				return;
			}
		}
	} // namespace core
} // namespace cube
