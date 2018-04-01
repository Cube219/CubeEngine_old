#include "ResourceManager.h"

#include "../LogWriter.h"
#include "BaseResource.h"

namespace cube
{
	namespace core
	{
		ResourceManager::ResourceManager(SPtr<platform::FileSystem>& fileSystem) : 
			mFileSystem(fileSystem)
		{
		}

		ResourceManager::~ResourceManager()
		{
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
			metaPath.append(CUBE_T(".meta"));
			SPtr<File> metaFile = mFileSystem->OpenFile(metaPath, FileAccessModeBits::Read);

			uint64_t size = metaFile->GetFileSize();
			char* metaString = (char*)malloc(size + 1);

			uint64_t readSize = 0;
			metaFile->Read(metaString, size, readSize);
			metaString[size] = '\0';

			Json metaJson = Json::parse(metaString);
			free(metaString);
			
			U8String resNameU8 = metaJson["res_name"];
			String resName = ToString(resNameU8);

			Resource* loadedRes = nullptr;
			// Find importer to import the resource
			bool isFindImporter = false;
			for(auto& importer : mImporters) {
				if(importer->GetResourceName() == resName) {
					SPtr<File> resFile = mFileSystem->OpenFile(path, FileAccessModeBits::Read);
					Json resInfo = metaJson["res_info"];

					loadedRes = importer->Import(resFile, resInfo);
					isFindImporter = true;
					break;
				}
			}

			if(loadedRes == nullptr) {
				if(isFindImporter == true)
					CUBE_LOG(LogType::Error, "Cannot find the importer whose res_name is \"{0}\".", resName);

				return nullptr;
			}

			{
				Lock lock(mLoadedResourcesMutex);

				mLoadedResources[path] = loadedRes; // TODO: UUID로 바꾸기
			}
			return RPtr<Resource>(loadedRes);
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
