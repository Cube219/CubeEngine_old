#include "ResourceManager.h"

#include "../LogWriter.h"
#include "BaseResource.h"
#include "FileSystem.h"

namespace cube
{
	namespace core
	{
		ResourceManager::ResourceManager()
		{
		}

		ResourceManager::~ResourceManager()
		{
			UnloadUnusedResources();
		}
		void ResourceManager::RegisterImporter(UPtr<ResourceImporter> importer)
		{
			mImporters.push_back(std::move(importer));
		}
		
		RPtr<Resource> ResourceManager::LoadResource(const String& path)
		{
			using namespace platform;

			// Check if the resource is already loaded
			{
				Lock lock(mLoadedResourcesMutex);

				auto findIter = mLoadedResources.find(path); // TODO: UUID로 바꾸기
				if(findIter != mLoadedResources.end()) {
					RPtr<Resource> resPtr(findIter->second);
					return resPtr;
				}
			}

			// Get a metadata
			String metaPath = path;
			metaPath.append(CUBE_T(".cmeta"));
			SPtr<File> metaFile = platform::FileSystem::OpenFile(metaPath, FileAccessModeBits::Read);

			uint64_t size = metaFile->GetFileSize();
			char* metaString = (char*)malloc(size + 1);

			uint64_t readSize = 0;
			metaFile->Read(metaString, size, readSize);
			metaString[size] = '\0';

			Json metaJson = Json::parse(metaString);
			free(metaString);
			
			U8String importerNameU8 = metaJson["importer_name"];
			String importerName = ToString(importerNameU8);

			Resource* loadedRes = nullptr;
			// Find importer to import the resource
			bool isFindImporter = false;
			for(auto& importer : mImporters) {
				if(importer->GetName() == importerName) {
					SPtr<File> resFile = platform::FileSystem::OpenFile(path, FileAccessModeBits::Read);
					Json info = metaJson["info"];

					loadedRes = importer->Import(resFile, info);
					isFindImporter = true;
					break;
				}
			}

			if(loadedRes == nullptr) {
				if(isFindImporter == true)
					CUBE_LOG(LogType::Error, "Cannot find the importer whose name is \"{0}\".", importerName);

				return nullptr;
			}

			{
				Lock lock(mLoadedResourcesMutex);

				mLoadedResources[path] = loadedRes; // TODO: UUID로 바꾸기
			}
			return RPtr<Resource>(loadedRes);
		}
		
		void ResourceManager::ReleaseResource(const RPtr<Resource>& res)
		{
			Lock lock(mLoadedResourcesMutex);
		}

		void ResourceManager::UnloadUnusedResources()
		{
			Lock lock(mLoadedResourcesMutex);

			Vector<String> resToUnload;
			for(auto iter = mLoadedResources.begin(); iter != mLoadedResources.end(); iter++) {
				if(iter->second->mRefCount == 0) {
					resToUnload.push_back(iter->first);
				}
			}

			for(auto& res : resToUnload) {
				delete mLoadedResources[res];
				mLoadedResources.erase(res);
			}
		}
	} // namespace core
} // namespace cube
