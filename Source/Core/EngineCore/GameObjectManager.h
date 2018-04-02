#pragma once

#include "EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT GameObjectManager
		{
		public:
			GameObjectManager();
			~GameObjectManager();

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
