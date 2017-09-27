#include "GameObject.h"

#include "Renderer\Renderer3D.h"

namespace cube
{
	namespace core
	{
		GameObject::GameObject() :
			mPosition({0.0f, 0.0f, 0.0f}), mRotation({0.0f, 0.0f, 0.0f}), mScale({1.0f, 1.0f, 1.0f}),
			mIsTransformChanged(true)
		{
		}

		GameObject::GameObject(SPtr<Renderer3D> renderer3D) : GameObject()
		{
			mRenderer3D = renderer3D;
		}

		GameObject::~GameObject()
		{
		}

		void GameObject::SetPosition(Vector3 position)
		{
			mPosition = position;
			mIsTransformChanged = true;
		}

		void GameObject::SetRotation(Vector3 rotation)
		{
			mRotation = rotation;
			mIsTransformChanged = true;
		}

		void GameObject::SetScale(Vector3 scale)
		{
			mScale = scale;
			mIsTransformChanged = true;
		}

		void GameObject::Update()
		{
			// Update model matrix
			if(mIsTransformChanged == true) {
				mModelMatrix[0][3] = mPosition.x;
				mModelMatrix[1][3] = mPosition.y;
				mModelMatrix[2][3] = mPosition.z;

				// (          cosYcosZ                  -cosYsinZ             sinY   )
				// (  sinXsinYcosZ + cosXsinZ   -sinXsinYsinZ + cosXcosZ   -sinXcosY )
				// ( -cosXsinYcosZ + sinXsinZ    cosXsinYsinZ + sinXcosZ    cosXcosY )
				float radX = glm::radians(mRotation.x);
				float radY = glm::radians(mRotation.y);
				float radZ = glm::radians(mRotation.z);

				float sinX = glm::sin(radX);
				float cosX = glm::cos(radX);
				float sinY = glm::sin(radY);
				float cosY = glm::cos(radY);
				float sinZ = glm::sin(radZ);
				float cosZ = glm::cos(radZ);

				mModelMatrix[0][0] = cosY * cosZ;
				mModelMatrix[0][1] = -cosY * sinZ;
				mModelMatrix[0][2] = sinY;
				mModelMatrix[1][0] = (sinX * sinY * cosZ) + (cosX * sinZ);
				mModelMatrix[1][1] = (-sinX * sinY * sinZ) + (cosX * cosZ);
				mModelMatrix[1][2] = -sinX * cosY;
				mModelMatrix[2][0] = (-cosX * sinY * cosZ) + (sinX * sinZ);
				mModelMatrix[2][1] = (cosX * sinY * sinZ) + (sinX * cosZ);
				mModelMatrix[2][2] = cosX * cosY;

				mRenderer3D->SetModelMatrix(mModelMatrix);

				mIsTransformChanged = false;
			}
		}
	}
}