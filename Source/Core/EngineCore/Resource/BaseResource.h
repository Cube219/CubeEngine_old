#pragma once

#include "../EngineCoreHeader.h"

#include "Base/Json.h"
#include "../Thread/MutexLock.h"
#include "FileSystem.h"

namespace cube
{
	class ENGINE_CORE_EXPORT ResourceImporter
	{
	public:
		ResourceImporter(){ }
		virtual ~ResourceImporter(){ }

		virtual Resource* Import(SPtr<platform::File>& file, Json info) = 0;

		const String& GetName() const { return mName; }

	protected:
		String mName;
	};

	class ENGINE_CORE_EXPORT Resource
	{
	public:
		Resource():
			mRefCount(0)
		{ }
		virtual ~Resource(){ }

	protected:
		friend class ResourceManager;
		template <typename T>
		friend class ResourcePointer;

		Atomic<Uint32> mRefCount;
	};
} // namespace cube
