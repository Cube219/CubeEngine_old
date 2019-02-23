#pragma once

#include "EngineCoreHeader.h"

#include "BasicHandler.h"
#include "Base/Vector.h"
#include "Base/Matrix.h"

namespace cube
{
	class ENGINE_CORE_EXPORT GameObject
	{
	public:
		static HGameObject Create();

	public:
		GameObject();
		~GameObject();

		void SetPosition(Vector3 position);
		void SetRotation(Vector3 rotation);
		void SetScale(Vector3 scale);
			
		Vector3 GetPosition() const { return mPosition; }
		Vector3 GetRotation() const { return mRotation; }
		Vector3 GetScale() const { return mScale; }

		Vector3 GetForward() const { return mForward; }
		Vector3 GetUp() const { return mUp; }
		Vector3 GetRight() const { return mRight; }

		HComponent GetComponent(StringRef name);
		template <typename T>
		BasicHandler<T> GetComponent()
		{
			const String& nameToGet = T::GetName();
			return GetComponent(nameToGet).Cast<T>();
		}

		HComponent AddComponent(StringRef name);
		template <typename T>
		BasicHandler<T> AddComponent()
		{
			const String& nameToAdd = T::GetName();
			return AddComponent(nameToAdd).Cast<T>();
		}

		void Start();

		void Update(float dt);

		void Destroy();

	private:
		friend class GameObjectManager;
		friend class Renderer3DComponent;

		Uint32 mID;
		HGameObject mMyHandler;

		SPtr<Renderer3D> mRenderer3D;

		// Variables related to transform
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		bool mIsTransformChanged;
		Matrix mScaleMatrix;
		Matrix mModelMatrix;

		Vector3 mForward;
		Vector3 mUp;
		Vector3 mRight;

		Vector<HComponent> mComponents;
	};
} // namespace cube
