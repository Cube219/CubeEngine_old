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
			ComponentManager() {}
			~ComponentManager() {}

			ComponentManager(const ComponentManager& other) = delete;
			ComponentManager& operator=(const ComponentManager& rhs) = delete;
			ComponentManager(ComponentManager&& other) = delete;
			ComponentManager& operator=(ComponentManager&& rhs) = delete;

			void Initialize();
			void ShutDown();

			template <typename T>
			void RegisterComponent()
			{
				const String& name = T::GetName();

				CheckIfComponentExisted(name);

				mComponentCreators[name] = []() {
					auto comDataPtr = std::make_shared<BasicHandlerData<Component>>();
					comDataPtr->data = std::make_shared<T>();
					comDataPtr->data->mMyHandler = HComponent(comDataPtr);

					return HComponent(comDataPtr);
				};
			}

			HComponent CreateComponent(StringRef name);

		private:
			void CheckIfComponentExisted(StringRef name);

			HashMap<String, std::function<HComponent()>> mComponentCreators;
		};
	} // namespace core
} // namespace cube
