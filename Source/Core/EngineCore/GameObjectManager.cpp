#include "GameObjectManager.h"

#include "BasicHandler.h"
#include "GameObject.h"
#include "LogWriter.h"

namespace cube
{
	namespace core
	{
		GameObjectManager::GameObjectManager() :
			mNextID(1)
		{
		}

		GameObjectManager::~GameObjectManager()
		{
			for(auto& go : mGameObjects) {
				go.second->data = nullptr;
			}
		}

		HGameObject GameObjectManager::RegisterGameObject(SPtr<GameObject>& go)
		{
			CHECK(go->mID == 0, "Failed to register GameObject. Only GameObject with id=0 can be registered (id: {0})", go->mID);

			go->mID = mNextID;

			SPtr<GameObjectData> goDataPtr = std::make_shared<GameObjectData>();
			goDataPtr->data = std::move(go);

			mGameObjects[mNextID] = goDataPtr;

			mNextID++;

			goDataPtr->data->mMyHandler = HGameObject(goDataPtr);

			return HGameObject(goDataPtr);
		}

		void GameObjectManager::UnregisterGameObject(HGameObject& go)
		{
			uint32_t id = go->mID;

			auto goIter = mGameObjects.find(id);
			CHECK(goIter != mGameObjects.end(), "Cannot unregister GameObject. It is not registered.");

			goIter->second->data = nullptr;
			mGameObjects.erase(goIter);
		}

		void GameObjectManager::Start()
		{
			for(auto& go : mGameObjects) {
				go.second->data->Start();
			}
		}

		void GameObjectManager::Update(float dt)
		{
			for(auto& go : mGameObjects) {
				go.second->data->Update(dt);
			}
		}
	} // namespace core
} // namespace cube
