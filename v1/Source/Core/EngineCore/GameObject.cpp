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
	HGameObject GameObject::Create()
	{
		GameObjectManager& manager = ECore().GetGameObjectManager();

		return manager.RegisterGameObject(std::make_unique<GameObject>());
	}

	GameObject::GameObject() :
		mPosition(0.0f, 0.0f, 0.0f), mRotation(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f),
		mIsModelMatrixChanged(true), mIsModelMatirxDirty(true),
		mForward(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mRight(1.0f, 0.0f, 0.0f)
	{
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::SetPosition(Vector3 position)
	{
		mPosition = position;
		mIsModelMatirxDirty = true;
	}

	void GameObject::SetRotation(Vector3 rotation)
	{
		mRotation = rotation;
		mIsModelMatirxDirty = true;
	}

	void GameObject::SetScale(Vector3 scale)
	{
		mScale = scale;
		mIsModelMatirxDirty = true;
	}

	HComponent GameObject::GetComponent(StringRef name)
	{
		HComponent componentToGet;

		for(auto& c : mComponents) {
			if(c->GetName() == name.GetString()) {
				componentToGet = c;
				break;
			}
		}

		return componentToGet;
	}

	HComponent GameObject::AddComponent(StringRef name)
	{
		for(auto& com : mComponents) {
			CHECK(com->GetName() != name.GetString(), "Cannot add component '{0}'. The component already exists.", name.GetString());
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
		mIsModelMatrixChanged = false;

		for(auto& com : mComponents) {
			com->OnUpdate(dt);
		}

		// Update model matrix
		if(mIsModelMatirxDirty == true) {
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

			for(auto& com : mComponents) {
				com->OnTransformChanged();
			}

			mIsModelMatirxDirty = false;
			mIsModelMatrixChanged = true;
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

		ECore().GetGameObjectManager().UnregisterGameObject(GetHandler());
	}
} // namespace cube
