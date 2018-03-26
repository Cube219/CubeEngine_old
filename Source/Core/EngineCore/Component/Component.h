#pragma once

#include "../EngineCoreHeader.h"

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

			void AttachGameObject(GameObject* gameObject){ mAttachedGameObject = gameObject; }

			GameObject* mAttachedGameObject;
		};
	} // namespace core
} // namespace cube
