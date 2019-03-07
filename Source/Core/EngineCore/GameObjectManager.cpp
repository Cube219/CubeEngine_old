#include "GameObjectManager.h"

#include "GameObject.h"
#include "LogWriter.h"

namespace cube
{
	void GameObjectManager::Initialize()
	{
	}

	void GameObjectManager::ShutDown()
	{
		mGameObjects.clear();
		mGameObjectTable.ReleaseAll();
	}

	HGameObject GameObjectManager::RegisterGameObject(UPtr<GameObject>&& go)
	{
		CHECK(go->GetID() == Uint64InvalidValue, "Failed to register GameObject. It is already registered. (id: {0})", go->GetID());

		HGameObject handler = mGameObjectTable.CreateNewHandler(std::move(go));
		mGameObjects.push_back(handler);

		return handler;
	}

	void GameObjectManager::UnregisterGameObject(HGameObject& go)
	{
		auto findIter = std::find(mGameObjects.begin(), mGameObjects.end(), go);
		CHECK(findIter != mGameObjects.end(), "Cannot unregister GameObject. It is not registered.");
		mGameObjects.erase(findIter);

		mGameObjectTable.ReleaseHandler(go);
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
