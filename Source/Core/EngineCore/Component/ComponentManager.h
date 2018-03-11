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
			void RegisterComponent()
			{
				const String& name = T::GetName();

				CheckIfComponentExisted(name);

				mComponentCreators[name] = []() {
					SPtr<T> c(new T());
					return c;
				};
			}

			SPtr<Component> CreateComponent(const String& name);

		private:
			void CheckIfComponentExisted(const String& name);

			HashMap<String, std::function<SPtr<Component>()>> mComponentCreators;
		};
	}
}
