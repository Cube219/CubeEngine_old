#pragma once

#include "EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT GameObjectManager
		{
		public:
			GameObjectManager() : mNextID(1)
			{}
			~GameObjectManager() {}

			GameObjectManager(const GameObjectManager& other) = delete;
			GameObjectManager& operator=(const GameObjectManager& rhs) = delete;
			GameObjectManager(GameObjectManager&& other) = delete;
			GameObjectManager& operator=(GameObjectManager&& rhs) = delete;

			void Initialize();
			void ShutDown();

			HGameObject RegisterGameObject(SPtr<GameObject>& go);
			void UnregisterGameObject(HGameObject& go);

			void Start();

			void Update(float dt);

		private:
			friend class GameObject;

			uint32_t mNextID;

			HashMap<uint32_t, SPtr<GameObjectData>> mGameObjects;
		};
	} // namespace core
} // namespace cube
