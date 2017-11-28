#include "ResourceManager.h"

namespace cube
{
	namespace core
	{
		ResourceManager::ResourceManager(SPtr<platform::BasePlatformFileSystem>& fileSystem) : 
			mFileSystem(fileSystem)
		{
		}

		ResourceManager::~ResourceManager()
		{
		}

		ResourcePointer<BaseResource> ResourceManager::LoadResource(WString& path)
		{
			ResourcePointer<BaseResource> resPtr;

			{
				Lock lock(mLoadedResourcesMutex);

				// Check if the resource is already loaded
				auto findIter = mLoadedResources.find(path); // TODO: UUID로 바꾸기
				if(findIter != mLoadedResources.end()) {
					resPtr.rawPtr = new BaseResource(findIter->second);
					return std::move(resPtr);
				}
			}

			// Load resource from file
			auto resFile = mFileSystem->OpenFile(path, platform::FileAccessModeBits::Read);
			uint64_t resFileSize = resFile->GetFileSize();

			ResourceRawData* rawData = new ResourceRawData(resFileSize);

			uint64_t readSize = 0;
			resFile->Read(rawData->mRawData, rawData->mSize, readSize);

			{
				Lock lock(mLoadedResourcesMutex);

				mLoadedResources[path] = rawData; // TODO: UUID로 바꾸기
			}

			resPtr.rawPtr = new BaseResource(rawData);
			return std::move(resPtr);
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