#include "GameObject.h"

#include "Assertion.h"
#include "EngineCore.h"
#include "Base/Math.h"
#include "Base/MatrixUtility.h"
#include "GameObjectManager.h"
#include "Component/Component.h"
#include "Component/ComponentManager.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/CameraRenderer3D.h"
#include "CubeEngine/Component/Renderer3DComponent.h"

namespace cube
{
	namespace core
	{
		HGameObject GameObject::Create()
		{
			GameObjectManager& manager = ECore().GetGameObjectManager();

			SPtr<GameObject> go = std::make_shared<GameObject>();
			return manager.RegisterGameObject(go);
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
				CHECK(com->GetName() != name, "Cannot add component \"{0}\". The component already exists.", name);
			}

			HComponent c = ECore().GetComponentManager().CreateComponent(name);
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

			// Update model matrix
			if(mIsTransformChanged == true) {
				mModelMatrix = MatrixUtility::GetTranslation(mPosition);
				mModelMatrix *= MatrixUtility::GetRotationXYZ(mRotation * (Math::Pi / 180.0f));

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

				for(auto& com : mComponents) {
					com->OnTransformChanged();
				}

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

			ECore().GetGameObjectManager().UnregisterGameObject(mMyHandler);
		}
	} // namespace core
} // namespace cube
