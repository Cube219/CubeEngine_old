#pragma once

#include "../EngineCoreHeader.h"

#include "BasePlatform/BasePlatformFileSystem.h"
#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ResourceManager
		{
		public:
			ResourceManager(SPtr<platform::BasePlatformFileSystem>& fileSystem);
			~ResourceManager();

			SPtr<BaseResource> LoadResource(WString& path);

			template <typename T>
			SPtr<T> LoadResource(WString& path) // TODO: 이걸 Main으로
			{
				return LoadResource(path)->CastResource<T>();
			}

			void UnloadUnusedResources();

		private:
			Mutex mLoadedResourcesMutex;
			HashMap<WString, ResourceRawData*> mLoadedResources;

			SPtr<platform::BasePlatformFileSystem> mFileSystem;
		};
	}
}