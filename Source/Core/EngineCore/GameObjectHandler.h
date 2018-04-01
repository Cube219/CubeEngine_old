#pragma once

#include "EngineCoreHeader.h"

namespace cube
{
	namespace core
	{
		struct GameObjectHandlerData
		{
			UPtr<GameObject> data;
		};

		class ENGINE_CORE_EXPORT GameObjectHandler
		{
		public:
			GameObjectHandler() : mData(nullptr) {}
			~GameObjectHandler() {}

			GameObjectHandler(SPtr<GameObjectHandlerData>& data) : mData(data) {}

			GameObject* operator->() const
			{
				return &*(mData->data);
			}
			GameObject& operator*() const
			{
				return *(mData->data);
			}

		private:
			friend class GameObject;

			// Can access in GameObject
			void Destroy();
			bool IsDestroyed() const
			{
				return mData->data != nullptr;
			}

			SPtr<GameObjectHandlerData> mData;
		};

		using HGameObject = GameObjectHandler;

	} // namespace cube
} // namespace core
