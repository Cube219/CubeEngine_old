#include "GameObjectHandler.h"

#include "EngineCore.h"
#include "GameObjectManager.h"

namespace cube
{
	namespace core
	{
		void GameObjectHandler::Destroy()
		{
			if(mData != nullptr && mData->data != nullptr) {
				ECore()->GetGameObjectManager()->UnregisterGameObject(*this);
			}
		}
	} // namespace core
} // namespace cube
