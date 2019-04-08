#include "CubeEngine/CubeEngine.h"
#include "EngineCore/Renderer/Texture.h"
#include "EngineCore/Renderer/Material/Shader.h"
#include "EngineCore/Resource/ResourcePointer.h"
#include "EngineCore/Renderer/BaseMeshGenerator.h"
#include "EngineCore/Renderer/Material/Material.h"
#include "EngineCore/Renderer/Material/MaterialInstance.h"
#include "EngineCore/Renderer/Skybox/Skybox.h"
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

	HSkybox skybox;
	RPtr<Texture> rightSkyboxTexture;
	RPtr<Texture> leftSkyboxTexture;
	RPtr<Texture> topSkyboxTexture;
	RPtr<Texture> bottomSkyboxTexture;
	RPtr<Texture> backSkyboxTexture;
	RPtr<Texture> frontSkyboxTexture;

	void PrepareResources()
	{
		// Load mesh / texture
		boxMesh = BaseMeshGenerator::GetBoxMesh();
		sphereMesh = BaseMeshGenerator::GetSphereMesh();
		planeMesh = BaseMeshGenerator::GetPlaneMesh();
		nanosuitMesh = Mesh::Load(CUBE_T("../../../SampleResources/Models/nanosuit.obj"));

		texture = Texture::Load(CUBE_T("../../../SampleResources/Textures/TestTexture.png"));
		texture2 = Texture::Load(CUBE_T("../../../SampleResources/Textures/TestTexture2.png"));

		// Load shader
		vertexShader = Shader::Load(CUBE_T("../../../SampleResources/Shaders/Vertex.glsl"));

		fragmentShader = Shader::Load(CUBE_T("../../../SampleResources/Shaders/Fragment.glsl"));

		fragmentTexturedShader = Shader::Load(CUBE_T("../../../SampleResources/Shaders/Fragment-Textured.glsl"));

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
		matUBO.albedo = Vector4(1.0f, 0, 0, 1);
		matUBO.metallic = 0.5f;
		matUBO.roughness = 0.5f;
		
		for(int i = 0; i <= 5; i++) {
			for(int j = 0; j <= 5; j++) {
				HMaterialInstance ins = material->CreateInstance();
				matUBO.metallic = 0.20f * j;
				matUBO.roughness = 0.20f * i + 0.1f;

				ins->SetParameterData(CUBE_T("UBO"), matUBO);
				ins->SetParameterData(CUBE_T("Texture"), texture);

				materialInses.push_back(ins);
			}
		}

		armTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/arm_showroom_spec.png"));
		bodyTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/body_showroom_spec.png"));
		glassTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/glass_dif.png"));
		handTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/hand_showroom_spec.png"));
		helmetTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/helmet_showroom_spec.png"));
		legTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/nanosuit/leg_showroom_spec.png"));

		matUBO.metallic = 0.5f;
		matUBO.roughness = 0.1f;

		armMatIns = materialTextured->CreateInstance();
		armMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		armMatIns->SetParameterData(CUBE_T("Texture"), armTexture);

		bodyMatIns = materialTextured->CreateInstance();
		bodyMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		bodyMatIns->SetParameterData(CUBE_T("Texture"), bodyTexture);

		glassMatIns = materialTextured->CreateInstance();
		glassMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		glassMatIns->SetParameterData(CUBE_T("Texture"), glassTexture);

		handMatIns = materialTextured->CreateInstance();
		handMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		handMatIns->SetParameterData(CUBE_T("Texture"), handTexture);

		helmetMatIns = materialTextured->CreateInstance();
		helmetMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		helmetMatIns->SetParameterData(CUBE_T("Texture"), helmetTexture);

		legMatIns = materialTextured->CreateInstance();
		legMatIns->SetParameterData(CUBE_T("UBO"), matUBO);
		legMatIns->SetParameterData(CUBE_T("Texture"), legTexture);
		
		// Load skybox
		rightSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/right.jpg"));
		leftSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/left.jpg"));
		topSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/top.jpg"));
		bottomSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/bottom.jpg"));
		backSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/back.jpg"));
		frontSkyboxTexture = Texture::Load(CUBE_T("../../../SampleResources/Textures/cubemap/skybox/front.jpg"));

		SkyboxInitializer skyboxInit;
		skyboxInit.textureRight = rightSkyboxTexture;
		skyboxInit.textureLeft = leftSkyboxTexture;
		skyboxInit.textureTop = topSkyboxTexture;
		skyboxInit.textureBottom = bottomSkyboxTexture;
		skyboxInit.textureBack = backSkyboxTexture;
		skyboxInit.textureFront = frontSkyboxTexture;
		skybox = Skybox::Create(skyboxInit);
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
		go->SetPosition(Vector3(10, 10, -1));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);
		
		go = GameObject::Create();
		go->SetPosition(Vector3(-10, 10, -1));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);

		go = GameObject::Create();
		go->SetPosition(Vector3(10, -10, -1));
		pointLight = go->AddComponent<PointLightComponent>();
		pointLight->SetColor(Vector4(23.47f, 21.31f, 20.79f, 1));
		pointLightGameObjects.push_back(go);

		go = GameObject::Create();
		go->SetPosition(Vector3(-10, -10, -1));
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
		
		skybox->Destroy();

		rightSkyboxTexture = nullptr;
		leftSkyboxTexture = nullptr;
		topSkyboxTexture = nullptr;
		bottomSkyboxTexture = nullptr;
		frontSkyboxTexture = nullptr;
		backSkyboxTexture = nullptr;

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
