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

			void RegisterImporter(UPtr<ResourceImporter> importer);

			RPtr<Resource> LoadResource(WString& path);

			template <typename T>
			RPtr<T> LoadResource(WString& path)
			{
				return LoadResource(path).Cast<T>();
			}

			void UnloadUnusedResources();

		private:
			Vector<UPtr<ResourceImporter>> mImporters;

			Mutex mLoadedResourcesMutex;
			HashMap<WString, Resource*> mLoadedResources;

			SPtr<platform::BasePlatformFileSystem> mFileSystem;
		};
	}
}