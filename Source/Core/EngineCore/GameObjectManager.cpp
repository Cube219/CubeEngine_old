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

		HGameObject GameObjectManager::RegisterGameObject(UPtr<GameObject>& go)
		{
			if(go->mID != 0) {
				CUBE_LOG(LogType::Error, "Cannot register GameObject. Only GameObject with id=0 can be registered (id: {0})", go->mID);
				return HGameObject();
			}

			go->mID = mNextID;

			SPtr<BasicHandlerData<GameObject>> goHandlerData = std::make_shared<BasicHandlerData<GameObject>>();
			goHandlerData->data = std::move(go);

			mGameObjects[mNextID] = goHandlerData;

			mNextID++;

			goHandlerData->data->mMyHandler = HGameObject(goHandlerData);

			return HGameObject(goHandlerData);
		}

		void GameObjectManager::UnregisterGameObject(HGameObject& go)
		{
			uint32_t id = go->mID;

			auto goIter = mGameObjects.find(id);
			if(goIter == mGameObjects.end()) {
				CUBE_LOG(LogType::Error, "Cannot unregister GameObject. It is not registered.");
				return;
			}

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
