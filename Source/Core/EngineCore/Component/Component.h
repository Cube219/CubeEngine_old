#pragma once

#include "../EngineCoreHeader.h"
#include "../BasicHandler.h"

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT Component
		{
		public:
			static const String& GetName() { return mName; }

		private:
			static String mName;

		public:
			Component();
			virtual ~Component();

			virtual void OnInit() = 0;
			virtual void OnUpdate(float dt) = 0;
			virtual void OnDestroy() = 0;

			GameObject* GetGameObject() const { return mAttachedGameObject; }

		private:
			friend class GameObject;
			friend class ComponentManager;

			void AttachGameObject(GameObject* gameObject){ mAttachedGameObject = gameObject; }
			void Destroy();

			HComponent mMyHandler;

			GameObject* mAttachedGameObject;
		};
	} // namespace core
} // namespace cube
