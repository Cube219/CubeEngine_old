#pragma once

#include "../EngineCoreHeader.h"

#include <functional>

#include "Base/format.h"
#include "../LogWriter.h"

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

				auto findIter = mComponentCreators.find(name);
				if(findIter != mComponentCreators.end()) {
					CUBE_LOG(LogType::Error, fmt::format(L"Component \"{0}\" is already registered", name));
					return;
				}

				mComponentCreators[name] = []() {
					SPtr<T> c(new T());
					return c;
				};
			}

			SPtr<Component> CreateComponent(const String& name);

		private:
			HashMap<String, std::function<SPtr<Component>()>> mComponentCreators;
		};
	}
}