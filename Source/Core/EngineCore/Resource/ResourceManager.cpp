#include "ResourceManager.h"

#include "BaseResource.h"

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

		SPtr<BaseResource> ResourceManager::LoadResource(WString& path)
		{
			// Check if the resource is already loaded
			{
				Lock lock(mLoadedResourcesMutex);

				auto findIter = mLoadedResources.find(path); // TODO: UUID�� �ٲٱ�
				if(findIter != mLoadedResources.end()) {
					SPtr<BaseResource> resPtr(new BaseResource(findIter->second));
					return resPtr;
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

				mLoadedResources[path] = rawData; // TODO: UUID�� �ٲٱ�
			}

			SPtr<BaseResource> resPtr(new BaseResource(rawData));
			return resPtr;
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