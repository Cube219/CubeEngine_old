#include "GameObject.h"

#include "LogWriter.h"
#include "EngineCore.h"
#include "GameObjectManager.h"
#include "Component/Component.h"
#include "Component/ComponentManager.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/CameraRenderer3D.h"
#include <gtc/matrix_transform.hpp>
#include "Base/MatrixUtility.h"
#include "CubeEngine/Component/Renderer3DComponent.h"

namespace cube
{
	namespace core
	{
		HGameObject GameObject::Create()
		{
			SPtr<GameObjectManager> manager = ECore()->GetGameObjectManager();

			SPtr<GameObject> go = std::make_shared<GameObject>();
			return manager->RegisterGameObject(go);
		}

		GameObject::GameObject() :
			mID(0), mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f),
			mIsTransformChanged(true),
			mForward(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mRight(1.0f, 0.0f, 0.0f)
		{
			mRenderer3D = nullptr;
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

		HComponent GameObject::GetComponent(const String& name)
		{
			HComponent componentToGet;

			for(auto& c : mComponents) {
				if(c->GetName() == name) {
					componentToGet = c;
					break;
				}
			}

			return componentToGet;
		}

		HComponent GameObject::AddComponent(const String& name)
		{
			for(auto& com : mComponents) {
				if(com->GetName() == name) {
					CUBE_LOG(LogType::Error, "Cannot add component \"{0}\". The component already exists", name);
					return HComponent();
				}
			}

			HComponent c = ECore()->GetComponentManager()->CreateComponent(name);
			c->AttachGameObject(this);
			mComponents.push_back(c);

			return c;
		}

		void GameObject::Start()
		{
			for(auto& com : mComponents) {
				com->OnInit();
			}
		}

		void GameObject::Update(float dt)
		{
			for(auto& com : mComponents) {
				com->OnUpdate(dt);
			}

			/*
			Float3 pos, rotation, scale;
			pos = mPosition.GetFloat3();
			rotation = mRotation.GetFloat3();
			scale = mScale.GetFloat3();*/

			// Update model matrix
			if(mIsTransformChanged == true) {
				// Position
				/*mModelMatrix[3][0] = pos.x;
				mModelMatrix[3][1] = -pos.y;
				mModelMatrix[3][2] = -pos.z; // Convert to left-hand coordinate

				// (          cosYcosZ                  -cosYsinZ             sinY   )
				// (  sinXsinYcosZ + cosXsinZ   -sinXsinYsinZ + cosXcosZ   -sinXcosY )
				// ( -cosXsinYcosZ + sinXsinZ    cosXsinYsinZ + sinXcosZ    cosXcosY )
				float radX = glm::radians(rotation.x);
				float radY = glm::radians(-rotation.y);
				float radZ = glm::radians(-rotation.z); // Convert to left-hand coordinate

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

				// Update the vectors of direction
				// TODO: Matrix 구현시 glm을 통하지 않고 바로 곱하게 만들기
				glm::vec4 forward(0.0f, 0.0f, 1.0f, 0.0f);
				glm::vec4 up(0.0f, 1.0f, 0.0f, 0.0f);
				glm::vec4 right(1.0f, 0.0f, 0.0f, 0.0f);

				forward = forward * mModelMatrix;
				up = up * mModelMatrix;
				right = right * mModelMatrix;
				mForward = Vector3(forward.x, forward.y, forward.z);
				mUp = Vector3(up.x, up.y, up.z);
				mRight = Vector3(right.x, right.y, right.z);

				// Scale
				mScaleMatrix[0][0] = scale.x;
				mScaleMatrix[1][1] = scale.y;
				mScaleMatrix[2][2] = scale.z;

				mModelMatrix *= mScaleMatrix;
				*/

				mModelMatrix = MatrixUtility::GetTranslation(mPosition);
				mModelMatrix *= MatrixUtility::GetRotationXYZ(mRotation * Math::Pi / 180.0f);

				// Update the vectors of direction
				Vector4 forward(0, 0, 1, 0);
				Vector4 up(0, 1, 0, 0);
				Vector4 right(1, 0, 0, 0);
				mForward = forward * mModelMatrix;
				mUp = up * mModelMatrix;
				mRight = right * mModelMatrix;

				mModelMatrix *= MatrixUtility::GetScale(mScale);

				if(mRenderer3D != nullptr)
					mRenderer3D->SetModelMatrix(mModelMatrix);

				mIsTransformChanged = false;
			}
		}

		void GameObject::Destroy()
		{
			for(auto& c : mComponents) {
				c->OnDestroy();
			}
			for(auto& c : mComponents) {
				c->Destroy();
			}

			ECore()->GetGameObjectManager()->UnregisterGameObject(mMyHandler);
		}
	} // namespace core
} // namespace cube
