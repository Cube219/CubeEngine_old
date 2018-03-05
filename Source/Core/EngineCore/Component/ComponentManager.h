#pragma once

#include "../EngineCoreHeader.h"

#include <functional>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ComponentManager
		{
		public:
			ComponentManager();
			~ComponentManager();

			template <typename T>
			void RegisterComponent();

			SPtr<Component> CreateComponent(const String& name);

		private:
			HashMap<String, std::function<SPtr<Component>()>> mComponentCreators;
		};
	}
}