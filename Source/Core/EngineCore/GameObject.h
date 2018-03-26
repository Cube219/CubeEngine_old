#pragma once

#include "EngineCoreHeader.h"

#include "Base/Vector.h"
#include <glm.hpp>

namespace cube
{
	namespace core
	{
		class ENGINE_CORE_EXPORT GameObject
		{
		public:
			GameObject();
			GameObject(SPtr<Renderer3D> renderer3D);
			GameObject(SPtr<CameraRenderer3D> cameraRenderer3D);
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

			SPtr<Renderer3D> GetRenderer() const { return mRenderer3D; }

			SPtr<Component> GetComponent(const String& name);
			template <typename T>
			SPtr<T> GetComponent()
			{
				const String& nameToGet = T::GetName();
				return DPCast(T)(GetComponent(nameToGet));
			}

			SPtr<Component> AddComponent(const String& name);
			template <typename T>
			SPtr<T> AddComponent()
			{
				const String& nameToAdd = T::GetName();
				return DPCast(T)(AddComponent(nameToAdd));
			}

			void Start();

			void Update(float dt);

		private:

			SPtr<Renderer3D> mRenderer3D;
			SPtr<CameraRenderer3D> mCameraRenderer3D;

			// Variables related to transform
			Vector3 mPosition;
			Vector3 mRotation;
			Vector3 mScale;
			bool mIsTransformChanged;
			glm::mat4 mScaleMatrix;
			glm::mat4 mModelMatrix;

			Vector3 mForward;
			Vector3 mUp;
			Vector3 mRight;

			Vector<SPtr<Component>> mComponents;
		};
	} // namespace core
} // namespace cube
