#pragma once

#include "../EngineCoreHeader.h"

#include "BaseResource.h"
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

			ResourcePointer<BaseResource> LoadResource(WString& path);

			template <typename T>
			ResourcePointer<T> LoadResource(WString& path)
			{
				ResourcePointer<T>::CastFromBase(LoadResource(path));
			}

			void UnloadUnusedResources();

		private:
			Mutex mLoadedResourcesMutex;
			HashMap<WString, ResourceRawData*> mLoadedResources;

			SPtr<platform::BasePlatformFileSystem> mFileSystem;
		};
	}
}