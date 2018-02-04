#include "GameObject.h"

#include "Renderer\Renderer3D.h"
#include <iostream>
#include <gtc\matrix_transform.hpp>
namespace cube
{
	namespace core
	{
		GameObject::GameObject() :
			mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f),
			mIsTransformChanged(true), mScaleMatrix(1.0f), mModelMatrix(1.0f)
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
			float pos[3], rotation[3], scale[3];
			mPosition.GetFloat3(pos);
			mRotation.GetFloat3(rotation);
			mScale.GetFloat3(scale);

			// Update model matrix
			if(mIsTransformChanged == true) {
				// Position
				mModelMatrix[3][0] = pos[0];
				mModelMatrix[3][1] = -pos[1];
				mModelMatrix[3][2] = -pos[2]; // Convert to left-hand coordinate

				// (          cosYcosZ                  -cosYsinZ             sinY   )
				// (  sinXsinYcosZ + cosXsinZ   -sinXsinYsinZ + cosXcosZ   -sinXcosY )
				// ( -cosXsinYcosZ + sinXsinZ    cosXsinYsinZ + sinXcosZ    cosXcosY )
				float radX = glm::radians(rotation[0]);
				float radY = glm::radians(-rotation[1]);
				float radZ = glm::radians(-rotation[2]); // Convert to left-hand coordinate

				float sinX = glm::sin(radX);
				float cosX = glm::cos(radX);
				float sinY = glm::sin(radY);
				float cosY = glm::cos(radY);
				float sinZ = glm::sin(radZ);
				float cosZ = glm::cos(radZ);

				mModelMatrix[0][0] = cosY * cosZ;
				mModelMatrix[1][0] = -cosY * sinZ;
				mModelMatrix[2][0] = sinY;
				mModelMatrix[0][1] = (sinX * sinY * cosZ) + (cosX * sinZ);
				mModelMatrix[1][1] = (-sinX * sinY * sinZ) + (cosX * cosZ);
				mModelMatrix[2][1] = -sinX * cosY;
				mModelMatrix[0][2] = (-cosX * sinY * cosZ) + (sinX * sinZ);
				mModelMatrix[1][2] = (cosX * sinY * sinZ) + (sinX * cosZ);
				mModelMatrix[2][2] = cosX * cosY;

				// Scale
				mScaleMatrix[0][0] = scale[0];
				mScaleMatrix[1][1] = scale[1];
				mScaleMatrix[2][2] = scale[2];

				mModelMatrix *= mScaleMatrix;

				mRenderer3D->SetModelMatrix(mModelMatrix);

				mIsTransformChanged = false;
			}
		}
	}
}