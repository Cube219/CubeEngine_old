#pragma once

#include "../EngineCoreHeader.h"

#include "../Thread/MutexLock.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT ResourceManager
		{
		public:
			ResourceManager() {}
			~ResourceManager() {}

			ResourceManager(const ResourceManager& other) = delete;
			ResourceManager& operator=(const ResourceManager& rhs) = delete;
			ResourceManager(ResourceManager&& other) = delete;
			ResourceManager& operator=(ResourceManager&& rhs) = delete;

			void Initialize();
			void ShutDown();

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
			friend class BaseMeshGenerator;

			Vector<UPtr<ResourceImporter>> mImporters;

			Mutex mLoadedResourcesMutex;
			HashMap<String, Resource*> mLoadedResources;
		};
	} // namespace core
} // namespace cube
