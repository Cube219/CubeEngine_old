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

			void RegisterGameObject(SPtr<GameObject>& go);
			void UnregisterGameObject(SPtr<GameObject>& go);

			void Start();

			void Update(float dt);

		private:
			friend class GameObject;

			uint32_t mNextID;

			HashMap<uint32_t, SPtr<GameObject>> mGameObjects;
		};
	} // namespace core
} // namespace cube
