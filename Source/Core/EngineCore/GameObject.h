#pragma once

#include "EngineCoreHeader.h"

#include <Base\Vector.h>
#include <glm.hpp>

namespace cube
{
	namespace core
	{
		class GameObject
		{
		public:
			GameObject();
			GameObject(SPtr<Renderer3D> renderer3D);
			~GameObject();

			void SetPosition(Vector3 position);
			void SetRotation(Vector3 rotation);
			void SetScale(Vector3 scale);
			
			Vector3 GetPosition() const;
			Vector3 GetRotation() const;
			Vector3 GetScale() const;

			SPtr<Renderer3D> GetRenderer() const;

			void Update();

		private:
			SPtr<Renderer3D> mRenderer3D;

			// Variables related to transform
			Vector3 mPosition;
			Vector3 mRotation;
			Vector3 mScale;
			bool mIsTransformChanged;
			glm::mat4 mScaleMatrix;
			glm::mat4 mModelMatrix;
		};

		inline Vector3 GameObject::GetPosition() const
		{
			return mPosition;
		}

		inline Vector3 GameObject::GetRotation() const
		{
			return mRotation;
		}

		inline Vector3 GameObject::GetScale() const
		{
			return mScale;
		}

		inline SPtr<Renderer3D> GameObject::GetRenderer() const
		{
			return mRenderer3D;
		}
	}
}