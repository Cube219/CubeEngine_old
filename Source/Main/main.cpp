#include "CubeEngine/CubeEngine.h"
#include "EngineCore/Renderer/Texture.h"
#include "EngineCore/Renderer/Material/Shader.h"
#include "EngineCore/Resource/ResourcePointer.h"
#include "EngineCore/Renderer/BaseMeshGenerator.h"
#include "EngineCore/Renderer/Material/Material.h"
#include "EngineCore/Renderer/Material/MaterialInstance.h"
#include "EngineCore/GameObject.h"
#include "CubeEngine/Component/CameraComponent.h"
#include "CubeEngine/Component/MoveComponent.h"
#include "CubeEngine/Component/Renderer3DComponent.h"

namespace cube
{
	SPtr<core::Mesh> boxMesh;
	SPtr<core::Mesh> planeMesh;
	core::RPtr<core::Texture> texture;
	core::RPtr<core::Texture> texture2;

	core::RPtr<core::Shader> vertexShader;
	core::RPtr<core::Shader> fragmentShader;
	core::HMaterial material;
	core::HMaterialInstance materialIns1;
	core::HMaterialInstance materialIns2;
	Vector<core::HGameObject> mGameObjects;
	core::HGameObject cameraGameObject;

	void PrepareResources()
	{
		using namespace core;

		// Create mesh / texture
		boxMesh = BaseMeshGenerator::GetBoxMesh();
		planeMesh = BaseMeshGenerator::GetPlaneMesh();

		String texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture.png");
		texture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture2.png");
		texture2 = Texture::Load(texturePath);

		// Load shader
		String shaderPath = CUBE_T("../../../SampleResources/Shaders/Vertex.glsl");
		vertexShader = Shader::Load(shaderPath);

		shaderPath = CUBE_T("../../../SampleResources/Shaders/Fragment.glsl");
		fragmentShader = Shader::Load(shaderPath);

		// Create material
		MaterialInitializer matInit;
		matInit.shaders.push_back(vertexShader);
		matInit.shaders.push_back(fragmentShader);
		matInit.parameters.push_back({CUBE_T("Texture"), MaterialParameterType::Texture, 0});
		material = Material::Create(matInit);

		// Create materialInstances
		materialIns1 = material->CreateInstance();
		String t = CUBE_T("Texture");
		materialIns1->SetParameterData<RPtr<Texture>>(t, texture);

		materialIns2 = material->CreateInstance();
		materialIns2->SetParameterData<RPtr<Texture>>(t, texture2);
	}

	void CreateGameObjects()
	{
		using namespace core;

		// Center
		auto go = GameObject::Create();
		Vector3 v(0, 0, 0);
		go->SetPosition(v);

		auto renderer = go->AddComponent<cube::Renderer3DComponent>();
		renderer->SetMesh(boxMesh);
		renderer->SetMaterialInstance(materialIns2);

		mGameObjects.push_back(go);

		// X Axis(1)
		go = GameObject::Create();
		v = Vector3(1, 0, 0);
		go->SetPosition(v);

		renderer = go->AddComponent<cube::Renderer3DComponent>();
		renderer->SetMesh(boxMesh);
		renderer->SetMaterialInstance(materialIns1);

		mGameObjects.push_back(go);

		// Y Axis(2)
		for(int i = 1; i <= 2; i++) {
			go = GameObject::Create();
			v = Vector3(0, i, 0);
			go->SetPosition(v);

			renderer = go->AddComponent<cube::Renderer3DComponent>();
			renderer->SetMesh(boxMesh);
			renderer->SetMaterialInstance(materialIns1);

			mGameObjects.push_back(go);
		}

		// Z Axis(3)
		for(int i = 1; i <= 3; i++) {
			go = GameObject::Create();
			v = Vector3(0, 0, i);
			go->SetPosition(v);

			renderer = go->AddComponent<cube::Renderer3DComponent>();
			renderer->SetMesh(boxMesh);
			renderer->SetMaterialInstance(materialIns1);

			mGameObjects.push_back(go);
		}

		// Plane
		go = GameObject::Create();
		v = Vector3(0, -2, 0);
		go->SetPosition(v);

		renderer = go->AddComponent<cube::Renderer3DComponent>();
		renderer->SetMesh(planeMesh);
		renderer->SetMaterialInstance(materialIns1);

		mGameObjects.push_back(go);

		cameraGameObject = GameObject::Create();
		cameraGameObject->SetPosition(Vector3(0, 0, -5));
		cameraGameObject->AddComponent<CameraComponent>();
		cameraGameObject->AddComponent<MoveComponent>();
	}

	void DestroyAll()
	{
		using namespace core;
		for(auto& go : mGameObjects) {
			go->Destroy();
		}
		cameraGameObject->Destroy();

		boxMesh = nullptr;
		texture = nullptr;
		texture2 = nullptr;
		vertexShader = nullptr;
		fragmentShader = nullptr;
		materialIns1->Destroy();
		materialIns2->Destroy();
		material->Destroy();
	}

	void MainImpl()
	{
		CubeEngineStartOption startOption;
		startOption.title = CUBE_T("Test title");
		startOption.windowWidth = 1024;
		startOption.windowHeight = 768;
		startOption.isWindowMode = true;

		CubeEngine::Start(startOption);

		PrepareResources();
		CreateGameObjects();

		CubeEngine::Run();

		DestroyAll();

		CubeEngine::Destroy();
	}
} // namespace cube

#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	cube::MainImpl();

	return 0;
}
#endif // _WIN32
