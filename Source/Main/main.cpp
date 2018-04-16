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
#include "CubeEngine/Component/DirectionalLightComponent.h"

#include "EngineCore/Renderer/Mesh.h"

namespace cube
{
	struct MaterialUBO
	{
		Vector4 albedo;
		float roughness;
		float metallic;
	};

	core::RPtr<core::Mesh> boxMesh;
	core::RPtr<core::Mesh> sphereMesh;
	core::RPtr<core::Mesh> planeMesh;
	core::RPtr<core::Texture> texture;
	core::RPtr<core::Texture> texture2;

	core::RPtr<core::Shader> vertexShader;
	core::RPtr<core::Shader> fragmentShader;
	core::HMaterial material;
	Vector<core::HMaterialInstance> materialInses;
	Vector<core::HGameObject> mGameObjects;
	core::HGameObject cameraGameObject;
	core::HGameObject dirLightGameObject;

	void PrepareResources()
	{
		using namespace core;

		// Create mesh / texture
		boxMesh = BaseMeshGenerator::GetBoxMesh();
		sphereMesh = BaseMeshGenerator::GetSphereMesh();
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
		matInit.parameters.push_back({CUBE_T("UBO"), MaterialParameterType::Data, sizeof(MaterialUBO)});
		matInit.parameters.push_back({CUBE_T("Texture"), MaterialParameterType::Texture, 1});
		material = Material::Create(matInit);

		// Create materialInstances
		MaterialUBO matUBO;
		matUBO.albedo = Vector4(0.5f, 0, 0, 1);
		matUBO.metallic = 0.5f;
		matUBO.roughness = 0.5f;
		
		String t;
		for(int i = 0; i <= 5; i++) {
			for(int j = 0; j <= 5; j++) {
				HMaterialInstance ins = material->CreateInstance();
				matUBO.metallic = 0.2f * i;
				matUBO.roughness = 0.2f * j;

				t = CUBE_T("UBO");
				ins->SetParameterData(t, matUBO);
				t = CUBE_T("Texture");
				ins->SetParameterData(t, texture);

				materialInses.push_back(ins);
			}
		}
	}

	void CreateGameObjects()
	{
		using namespace core;

		HGameObject go;
		Vector3 v;
		HRenderer3DComponent renderer;

		int insIndex = 0;

		for(int i = 0; i <= 5; i++) {
			for(int j = 0; j <= 5; j++) {
				go = GameObject::Create();
				v = Vector3(i * 2 - 5, j * 2 - 5, 0);
				go->SetPosition(v);

				renderer = go->AddComponent<cube::Renderer3DComponent>();
				renderer->SetMesh(sphereMesh);
				renderer->SetMaterialInstance(materialInses[insIndex]);
				insIndex++;
			}
		}

		// Camera
		cameraGameObject = GameObject::Create();
		cameraGameObject->SetPosition(Vector3(0, 0, -5));
		cameraGameObject->AddComponent<CameraComponent>();
		cameraGameObject->AddComponent<MoveComponent>();

		// Directional light
		dirLightGameObject = GameObject::Create();
		dirLightGameObject->SetRotation(Vector3(90, 0, 0));
		HDirectionalLightComponent dirLightCom = dirLightGameObject->AddComponent<DirectionalLightComponent>();
		dirLightCom->SetColor(Vector4(20, 20, 20, 1));
	}

	void DestroyAll()
	{
		using namespace core;
		for(auto& go : mGameObjects) {
			go->Destroy();
		}
		cameraGameObject->Destroy();
		dirLightGameObject->Destroy();

		boxMesh = nullptr;
		texture = nullptr;
		texture2 = nullptr;
		vertexShader = nullptr;
		fragmentShader = nullptr;
		for(auto& ins : materialInses) {
			ins->Destroy();
		}
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
