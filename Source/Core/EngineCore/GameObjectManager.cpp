#include "GameObjectManager.h"

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
		}

		void GameObjectManager::RegisterGameObject(SPtr<GameObject>& go)
		{
			if(go->mID != 0) {
				CUBE_LOG(LogType::Error, "Cannot register GameObject. Only GameObject with id=0 can be registered (id: {0})", go->mID);
				return;
			}

			mGameObjects[mNextID] = go;
			go->mID = mNextID;

			mNextID++;
		}

		void GameObjectManager::UnregisterGameObject(SPtr<GameObject>& go)
		{
			uint32_t id = go->mID;

			auto goIter = mGameObjects.find(id);
			if(goIter == mGameObjects.end()) {
				CUBE_LOG(LogType::Error, "Cannot unregister GameObject. It is not registered.");
				return;
			}

			mGameObjects.erase(goIter);
		}

		void GameObjectManager::Start()
		{
			for(auto& go : mGameObjects) {
				go.second->Start();
			}
		}

		void GameObjectManager::Update(float dt)
		{
			for(auto& go : mGameObjects) {
				go.second->Update(dt);
			}
		}
	} // namespace core
} // namespace cube
