#include "Base/BaseTypes.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/Renderer/Texture.h"
#include "EngineCore/Renderer/Material/Shader.h"
#include "EngineCore/Resource/ResourceManager.h"
#include "EngineCore/Resource/ResourcePointer.h"
#include "EngineCore/Renderer/RendererManager.h"
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
	core::RPtr<core::Texture> texture;
	core::RPtr<core::Texture> texture2;

	core::RPtr<core::Shader> vertexShader;
	core::RPtr<core::Shader> fragmentShader;
	SPtr<core::Material> material;
	SPtr<core::MaterialInstance> materialIns1;
	SPtr<core::MaterialInstance> materialIns2;
	Vector<SPtr<core::GameObject>> mGameObjects;
	SPtr<core::GameObject> cameraGameObject;

	void PrepareResources()
	{
		using namespace core;

		// Create mesh / texture
		boxMesh = BaseMeshGenerator::GetBoxMesh();

		String texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture.png");
		texture = ECore()->GetResourceManager()->LoadResource<Texture>(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture2.png");
		texture2 = ECore()->GetResourceManager()->LoadResource<Texture>(texturePath);

		// Load shader
		String shaderPath = CUBE_T("../../../SampleResources/Shaders/Vertex.glsl");
		vertexShader = ECore()->GetResourceManager()->LoadResource<Shader>(shaderPath);

		shaderPath = CUBE_T("../../../SampleResources/Shaders/Fragment.glsl");
		fragmentShader = ECore()->GetResourceManager()->LoadResource<Shader>(shaderPath);

		// Create material
		MaterialInitializer matInit;
		matInit.shaders.push_back(vertexShader);
		matInit.shaders.push_back(fragmentShader);
		matInit.parameters.push_back({CUBE_T("Texture"), MaterialParameterType::Texture, 0});
		material = std::make_shared<Material>(ECore()->GetRendererManager()->GetRenderAPI(), matInit);
		ECore()->GetRendererManager()->RegisterMaterial(material);

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

		int flag = -1;
		// Create gameobjects
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				for(int k = -1; k <= 1; k++) {
					if(i == 0 && j == 0 && k == 0)
						continue;

					auto go = GameObject::Create();
					Vector3 v(i * 2, j * 2, k * 2);
					go->SetPosition(v);

					auto renderer = go->AddComponent<Renderer3DComponent>();
					renderer->SetMesh(boxMesh);
					if(flag == -1)
						renderer->SetMaterialInstance(materialIns1);
					else
						renderer->SetMaterialInstance(materialIns2);
					flag *= -1;

					mGameObjects.push_back(go);
				}
			}
		}

		cameraGameObject = GameObject::Create();
		cameraGameObject->AddComponent<CameraComponent>();
		cameraGameObject->AddComponent<MoveComponent>();
	}
}

#ifdef _WIN32
#include <Windows.h>

#include "CubeEngine/CubeEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	using namespace cube;

	CubeEngineStartOption startOption;
	startOption.title = CUBE_T("Test title");
	startOption.windowWidth = 1024;
	startOption.windowHeight = 768;
	startOption.isWindowMode = true;

	CubeEngine::Start(startOption);

	PrepareResources();
	CreateGameObjects();

	CubeEngine::Run();

	CubeEngine::Destroy();

	return 0;
}
#endif // _WIN32
