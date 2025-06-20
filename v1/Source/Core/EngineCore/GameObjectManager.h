#pragma once

#include "EngineCoreHeader.h"

#include "GameObject.h"
#include "Handler.h"

namespace cube
{
	class ENGINE_CORE_EXPORT GameObjectManager
	{
	public:
		GameObjectManager() :
			mGameObjectTable(200)
		{}
		~GameObjectManager() {}

		GameObjectManager(const GameObjectManager& other) = delete;
		GameObjectManager& operator=(const GameObjectManager& rhs) = delete;
		GameObjectManager(GameObjectManager&& other) = delete;
		GameObjectManager& operator=(GameObjectManager&& rhs) = delete;

		void Initialize();
		void ShutDown();

		HGameObject RegisterGameObject(UPtr<GameObject>&& go);
		void UnregisterGameObject(HGameObject& go);

		void Start();

		void Update(float dt);

	private:
		friend class GameObject;

		HandlerTable mGameObjectTable;
		Vector<UPtr<GameObject>> mGameObjects;
	};
} // namespace cube
