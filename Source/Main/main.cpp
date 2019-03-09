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
#include "CubeEngine/Component/PointLightComponent.h"

#include "EngineCore/Renderer/Mesh.h"

namespace cube
{
	struct MaterialUBO
	{
		Vector4 albedo;
		float roughness;
		float metallic;
	};

	RPtr<Mesh> boxMesh;
	RPtr<Mesh> sphereMesh;
	RPtr<Mesh> planeMesh;
	RPtr<Mesh> nanosuitMesh;
	RPtr<Texture> texture;
	RPtr<Texture> texture2;

	RPtr<Shader> vertexShader;
	RPtr<Shader> fragmentShader;
	HMaterial material;
	RPtr<Shader> fragmentTexturedShader;
	HMaterial materialTextured;
	Vector<HMaterialInstance> materialInses;
	Vector<HGameObject> mGameObjects;
	HGameObject cameraGameObject;
	Vector<HGameObject> pointLightGameObjects;
	
	HGameObject nanosuitGameObject;
	HMaterialInstance armMatIns;
	RPtr<Texture> armTexture;
	HMaterialInstance bodyMatIns;
	RPtr<Texture> bodyTexture;
	HMaterialInstance glassMatIns;
	RPtr<Texture> glassTexture;
	HMaterialInstance handMatIns;
	RPtr<Texture> handTexture;
	HMaterialInstance helmetMatIns;
	RPtr<Texture> helmetTexture;
	HMaterialInstance legMatIns;
	RPtr<Texture> legTexture;

	void PrepareResources()
	{
		// Load mesh / texture
		boxMesh = BaseMeshGenerator::GetBoxMesh();
		sphereMesh = BaseMeshGenerator::GetSphereMesh();
		planeMesh = BaseMeshGenerator::GetPlaneMesh();
		String meshPath = CUBE_T("../../../SampleResources/Models/nanosuit.obj");
		nanosuitMesh = Mesh::Load(meshPath);

		String texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture.png");
		texture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture2.png");
		texture2 = Texture::Load(texturePath);

		// Load shader
		String shaderPath = CUBE_T("../../../SampleResources/Shaders/Vertex.glsl");
		vertexShader = Shader::Load(shaderPath);

		shaderPath = CUBE_T("../../../SampleResources/Shaders/Fragment.glsl");
		fragmentShader = Shader::Load(shaderPath);

		shaderPath = CUBE_T("../../../SampleResources/Shaders/Fragment-Textured.glsl");
		fragmentTexturedShader = Shader::Load(shaderPath);

		// Create material
		MaterialInitializer matInit;
		matInit.shaders.push_back(vertexShader);
		matInit.shaders.push_back(fragmentShader);
		matInit.parameters.push_back({CUBE_T("UBO"), MaterialParameterType::Data, sizeof(MaterialUBO)});
		matInit.parameters.push_back({CUBE_T("Texture"), MaterialParameterType::Texture, 1});
		material = Material::Create(matInit);

		matInit.shaders.clear();
		matInit.shaders.push_back(vertexShader);
		matInit.shaders.push_back(fragmentTexturedShader);
		materialTextured = Material::Create(matInit);

		// Create materialInstances
		MaterialUBO matUBO;
		matUBO.albedo = Vector4(0.5f, 0, 0, 1);
		matUBO.metallic = 0.5f;
		matUBO.roughness = 0.5f;
		
		String t;
		for(int i = 0; i <= 5; i++) {
			for(int j = 0; j <= 5; j++) {
				HMaterialInstance ins = material->CreateInstance();
				matUBO.metallic = 0.20f * j;
				matUBO.roughness = 0.20f * i + 0.1f;

				t = CUBE_T("UBO");
				ins->SetParameterData(t, matUBO);
				t = CUBE_T("Texture");
				ins->SetParameterData(t, texture);

				materialInses.push_back(ins);
			}
		}

		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/arm_showroom_spec.png");
		armTexture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/body_showroom_spec.png");
		bodyTexture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/glass_dif.png");
		glassTexture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/hand_showroom_spec.png");
		handTexture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/helmet_showroom_spec.png");
		helmetTexture = Texture::Load(texturePath);
		texturePath = CUBE_T("../../../SampleResources/Textures/nanosuit/leg_showroom_spec.png");
		legTexture = Texture::Load(texturePath);

		matUBO.metallic = 0.5f;
		matUBO.roughness = 0.1f;

		armMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		armMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		armMatIns->SetParameterData(t, armTexture);

		bodyMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		bodyMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		bodyMatIns->SetParameterData(t, bodyTexture);

		glassMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		glassMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		glassMatIns->SetParameterData(t, glassTexture);

		handMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		handMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		handMatIns->SetParameterData(t, handTexture);

		helmetMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		helmetMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		helmetMatIns->SetParameterData(t, helmetTexture);

		legMatIns = materialTextured->CreateInstance();
		t = CUBE_T("UBO");
		legMatIns->SetParameterData(t, matUBO);
		t = CUBE_T("Texture");
		legMatIns->SetParameterData(t, legTexture);
	}

	void CreateGameObjects()
	{
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
				renderer->SetMaterialInstance(materialInses[insIndex], 0);
				insIndex++;

				mGameObjects.push_back(go);
			}
		}

		// Camera
		cameraGameObject = GameObject::Create();
		cameraGameObject->SetPosition(Vector3(0, 0, -5));
		cameraGameObject->AddComponent<CameraComponent>();
		cameraGameObject->AddComponent<MoveComponent>();

		// Point light
		/*dirLightGameObject = GameObject::Create();
		dirLightGameObject->SetRotation(Vector3(90, 0, -45));
		HDirectionalLightComponent dirLightCom = dirLightGameObject->AddComponent<DirectionalLightComponent>();
		dirLightCom->SetColor(Vector4(10, 10, 10, 1));*/

		HPointLightComponent pointLight;

		go = GameObject::Create();
		go->SetPosition(Vector3(10, 10, -2));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);
		
		go = GameObject::Create();
		go->SetPosition(Vector3(-10, 10, -2));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);

		go = GameObject::Create();
		go->SetPosition(Vector3(10, -10, -2));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);

		go = GameObject::Create();
		go->SetPosition(Vector3(-10, -10, -2));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);

		// Nano suit
		nanosuitGameObject = GameObject::Create();
		nanosuitGameObject->SetPosition(Vector3(0, 0, -5));
		nanosuitGameObject->SetRotation(Vector3(0, 180, 0));

		renderer = nanosuitGameObject->AddComponent<cube::Renderer3DComponent>();
		renderer->SetMesh(nanosuitMesh);
		renderer->SetMaterialInstance(glassMatIns, 0);
		renderer->SetMaterialInstance(legMatIns, 1);
		renderer->SetMaterialInstance(handMatIns, 2);
		renderer->SetMaterialInstance(glassMatIns, 3);
		renderer->SetMaterialInstance(armMatIns, 4);
		renderer->SetMaterialInstance(helmetMatIns, 5);
		renderer->SetMaterialInstance(bodyMatIns, 6);
	}

	void DestroyAll()
	{
		for(auto& go : mGameObjects) {
			go->Destroy();
		}
		cameraGameObject->Destroy();
		for(auto& go : pointLightGameObjects) {
			go->Destroy();
		}
		nanosuitGameObject->Destroy();

		handTexture = nullptr;
		armTexture = nullptr;
		bodyTexture = nullptr;
		helmetTexture = nullptr;
		legTexture = nullptr;
		glassTexture = nullptr;

		nanosuitMesh = nullptr;

		boxMesh = nullptr;
		sphereMesh = nullptr;
		planeMesh = nullptr;
		texture = nullptr;
		texture2 = nullptr;
		vertexShader = nullptr;
		fragmentShader = nullptr;
		for(auto& ins : materialInses) {
			ins->Destroy();
		}
		material->Destroy();

		armMatIns->Destroy();
		bodyMatIns->Destroy();
		glassMatIns->Destroy();
		handMatIns->Destroy();
		helmetMatIns->Destroy();
		legMatIns->Destroy();
		materialTextured->Destroy();
	}

	void MainImpl()
	{
		CubeEngineStartOption startOption;
		startOption.title = CUBE_T("Test title");
		startOption.windowWidth = 1040;
		startOption.windowHeight = 807;
		startOption.isWindowMode = true;

		CubeEngine::Initialize(startOption);

		PrepareResources();
		CreateGameObjects();

		CubeEngine::Run();

		DestroyAll();

		CubeEngine::ShutDown();
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
