#include "GameObjectManager.h"

#include "LogWriter.h"

namespace cube
{
	void GameObjectManager::Initialize()
	{
	}

	void GameObjectManager::ShutDown()
	{
		mGameObjectTable.ReleaseAll();
		mGameObjects.clear();
	}

	HGameObject GameObjectManager::RegisterGameObject(UPtr<GameObject>&& go)
	{
		CHECK(go->GetID() == Uint64InvalidValue, "Failed to register GameObject. It is already registered. (id: {0})", go->GetID());

		mGameObjects.push_back(std::move(go));
		HGameObject handler = mGameObjectTable.CreateNewHandler(mGameObjects.back().get());

		return handler;
	}

	void GameObjectManager::UnregisterGameObject(HGameObject& go)
	{
		GameObject* pData = mGameObjectTable.ReleaseHandler(go);

		auto findIter = std::find_if(mGameObjects.cbegin(), mGameObjects.cend(), [pData](auto& element){
			return element.get() == pData;
		});
		CHECK(findIter != mGameObjects.end(), "Cannot unregister GameObject. It is not registered.");
		mGameObjects.erase(findIter);
	}

	void GameObjectManager::Start()
	{
		for(auto& go : mGameObjects) {
			go->Start();
		}
	}

	void GameObjectManager::Update(float dt)
	{
		for(auto& go : mGameObjects) {
			go->Update(dt);
		}
	}
} // namespace cube
