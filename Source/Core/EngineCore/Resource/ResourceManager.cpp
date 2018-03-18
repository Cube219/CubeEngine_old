#include "ResourceManager.h"

#include "../LogWriter.h"
#include "Base/format.h"
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
		
		RPtr<Resource> ResourceManager::LoadResource(WString& path)
		{
			using namespace platform;

			// Check if the resource is already loaded
			{
				Lock lock(mLoadedResourcesMutex);

				auto findIter = mLoadedResources.find(path); // TODO: UUID�� �ٲٱ�
				if(findIter != mLoadedResources.end()) {
					RPtr<Resource> resPtr(findIter->second);
					return resPtr;
				}
			}

			// Get a metadata
			WString metaPath = path;
			metaPath.append(L".meta");
			SPtr<File> metaFile = mFileSystem->OpenFile(metaPath, FileAccessModeBits::Read);

			uint64_t size = metaFile->GetFileSize();
			char* metaString = (char*)malloc(size + 1);

			uint64_t readSize = 0;
			metaFile->Read(metaString, size, readSize);
			metaString[size] = '\0';

			Json metaJson = Json::parse(metaString);
			free(metaString);
			
			String resName = metaJson["res_name"];

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
					CUBE_LOG(LogType::Error, L"Cannot find the importer whose res_name is \"{0}\".", resName);

				return nullptr;
			}

			{
				Lock lock(mLoadedResourcesMutex);

				mLoadedResources[path] = loadedRes; // TODO: UUID�� �ٲٱ�
			}
			return RPtr<Resource>(loadedRes);
		}
		
		void ResourceManager::UnloadUnusedResources()
		{
			Lock lock(mLoadedResourcesMutex);

			Vector<WString> resToUnload;
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
	}
}