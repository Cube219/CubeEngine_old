#pragma once

#include "../EngineCoreHeader.h"

#include "FileSystem.h"
#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ResourceManager
		{
		public:
			ResourceManager(SPtr<platform::FileSystem>& fileSystem);
			~ResourceManager();

			void RegisterImporter(UPtr<ResourceImporter> importer);

			RPtr<Resource> LoadResource(const String& path);

			template <typename T>
			RPtr<T> LoadResource(const String& path)
			{
				return LoadResource(path).Cast<T>();
			}

			void ReleaseResource(const RPtr<Resource>& res);

			void UnloadUnusedResources();

		private:
			Vector<UPtr<ResourceImporter>> mImporters;

			Mutex mLoadedResourcesMutex;
			HashMap<String, Resource*> mLoadedResources;

			SPtr<platform::FileSystem> mFileSystem;
		};
	} // namespace core
} // namespace cube
