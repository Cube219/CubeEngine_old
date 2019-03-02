#pragma once

#include "../EngineCoreHeader.h"

namespace cube
{
	class ENGINE_CORE_EXPORT StringManager
	{
	public:
		StringManager() {}
		~StringManager() {}

		StringManager(const StringManager& other) = delete;
		StringManager& operator=(const StringManager& rhs) = delete;
		StringManager(StringManager&& other) = delete;
		StringManager& operator=(StringManager&& rhs) = delete;

		void Initialize();
		void ShutDown();

	private:
	};
} // namespace cube
