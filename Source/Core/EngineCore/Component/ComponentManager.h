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
					auto comDataPtr = std::make_shared<BasicHandlerData<Component>>();
					comDataPtr->data = std::make_shared<T>();
					comDataPtr->data->mMyHandler = HComponent(comDataPtr);

					return HComponent(comDataPtr);
				};
			}

			HComponent CreateComponent(const String& name);

		private:
			void CheckIfComponentExisted(const String& name);

			HashMap<String, std::function<HComponent()>> mComponentCreators;
		};
	} // namespace core
} // namespace cube
