#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Json.h"
#include "../Thread/MutexLock.h"
#include "FileSystem.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ResourceImporter
		{
		public:
			ResourceImporter(){ }
			virtual ~ResourceImporter(){ }

			virtual Resource* Import(SPtr<platform::File>& file, Json info) = 0;

			const String2& GetResourceName() const { return mResName; }

		protected:
			String2 mResName;
		};

		class ENGINE_CORE_EXPORT Resource
		{
		public:
			virtual ~Resource(){ }

		protected:
			friend class ResourceManager;
			template <typename T>
			friend class ResourcePointer;

			Resource(){ }

			Atomic<uint32_t> mRefCount;
		};
	}
}
