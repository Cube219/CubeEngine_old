#include "EngineCore.h"

#include "Platform.h"
#include "Time/TimeManager.h"
#include "Time/GameTime.h"
#include "String/StringManager.h"
#include "Thread/ThreadManager.h"
#include "Base/format.h"
#include "LogWriter.h"
#include "Renderer/RendererManager.h"
#include "Renderer/Mesh.h"
#include "Renderer/BaseMeshGenerator.h"
#include "Renderer/Texture.h"
#include "Renderer/Material/Shader.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Material/MaterialInstance.h"
#include "Resource/ResourceManager.h"
#include "ModuleManager.h"
#include "GameObject.h"
#include "Component/ComponentManager.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/CameraRenderer3D.h"

namespace cube
{
	namespace core
	{
		EngineCore* EngineCore::mInstance = nullptr;

		EngineCore::EngineCore() :
			mFPSLimit(-1)
		{
			platform::Platform::SetLoopFunction(std::bind(&EngineCore::Loop, this));
			platform::Platform::SetResizeFunction(std::bind(&EngineCore::Resize, this, _1, _2));
		}

		EngineCore::~EngineCore()
		{
		}

		void EngineCore::Prepare()
		{
			LogWriter::Init();

			mTimeManager = std::make_unique<TimeManager>();
			mStringManager = std::make_unique<StringManager>();

			mRendererManager = std::make_unique<RendererManager>(RenderType::Vulkan);

			mResourceManager = std::make_unique<ResourceManager>(platform::Platform::GetFileSystem());
			mResourceManager->RegisterImporter(
				std::make_unique<TextureImporter>(mRendererManager->GetRenderAPI())
			);
			mResourceManager->RegisterImporter(
				std::make_unique<ShaderImporter>(mRendererManager->GetRenderAPI())
			);

			mThreadManager = std::make_shared<ThreadManager>();

			mModuleManager = std::make_shared<ModuleManager>(mThreadManager);
			mModuleManager->LoadModule(CUBE_T("InputModule"));

			mModuleManager->InitModules();

			mComponentManager = std::make_shared<ComponentManager>();

			// Create mesh / texture
			mBoxMesh = BaseMeshGenerator::GetBoxMesh();

			String2 texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture.png");
			mTexture = mResourceManager->LoadResource<Texture>(texturePath);
			texturePath = CUBE_T("../../../SampleResources/Textures/TestTexture2.png");
			mTexture2 = mResourceManager->LoadResource<Texture>(texturePath);

			// Load shader
			String2 shaderPath = CUBE_T("../../../SampleResources/Shaders/Vertex.glsl");
			mMaterialVertexShader = mResourceManager->LoadResource<Shader>(shaderPath);

			shaderPath = CUBE_T("../../../SampleResources/Shaders/Fragment.glsl");
			mMaterialFragmentShader = mResourceManager->LoadResource<Shader>(shaderPath);

			// Create material
			MaterialInitializer matInit;
			matInit.shaders.push_back(mMaterialVertexShader);
			matInit.shaders.push_back(mMaterialFragmentShader);
			matInit.parameters.push_back({CUBE_T("Texture"), MaterialParameterType::Texture, 0});
			mMaterial = std::make_shared<Material>(mRendererManager->GetRenderAPI(), matInit);
			mRendererManager->RegisterMaterial(mMaterial);

			// Create materialInstances
			mMaterialIns1 = mMaterial->CreateInstance();
			String2 t = CUBE_T("Texture");
			mMaterialIns1->SetParameterData<RPtr<Texture>>(t, mTexture);

			mMaterialIns2 = mMaterial->CreateInstance();
			mMaterialIns2->SetParameterData<RPtr<Texture>>(t, mTexture2);

			int flag = -1;
			// Create gameobjects
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					for(int k = -1; k <= 1; k++) {
						if(i == 0 && j == 0 && k == 0)
							continue;

						auto go = std::make_shared<GameObject>(mRendererManager->CreateRenderer3D());
						Vector3 v(i*2, j*2, k*2);
						go->SetPosition(v);

						auto renderer = go->GetRenderer();
						renderer->SetMesh(mBoxMesh);
						if(flag == -1)
							renderer->SetMaterialInstance(mMaterialIns1);
						else
							renderer->SetMaterialInstance(mMaterialIns2);
						flag *= -1;

						mGos.push_back(go);

						mRendererManager->RegisterRenderer3D(renderer);
					}
				}
			}

			mCameraGo = std::make_shared<GameObject>(mRendererManager->GetCameraRenderer3D());
		}

		void EngineCore::Run()
		{
			mTimeManager->Start();

			mCameraGo->Start();

			platform::Platform::StartLoop();
		}

		float EngineCore::GetCurrentFPS()
		{
			// TODO: 더 좋은 방법으로 개선
			return 1.0f / mTimeManager->GetGlobalGameTime()->GetDeltaTime();
		}

		void EngineCore::SetFPSLimit(int limit)
		{
			mFPSLimit = limit;
		}

		void EngineCore::Loop()
		{
			mTimeManager->Update();

			double currentTime = mTimeManager->GetSystemTime(); // For limit FPS 

			float dt = mTimeManager->GetGlobalGameTime()->GetDeltaTime();

			mModuleManager->UpdateAllModules(dt);

			for(auto& go : mGos) {
				go->Update(dt);
			}

			mCameraGo->Update(dt);
			
			mRendererManager->DrawAll();

			// Limit FPS
			if(mFPSLimit > 0) {
				double nextTime = currentTime + (1.0 / mFPSLimit);

				currentTime = mTimeManager->GetSystemTime();

				double waitTime = nextTime - currentTime;

				if(waitTime > 0.1) { // TODO: 적절한 수치를 찾기
					platform::Platform::Sleep(SCast(int)(waitTime * 1000));
				} else if(waitTime > 0.0) {
					while(nextTime > currentTime) {
						currentTime = mTimeManager->GetSystemTime();
					}
				}
			}
		}

		void EngineCore::Resize(uint32_t width, uint32_t height)
		{
			mRendererManager->Resize(width, height);
		}

		EngineCore* ECore()
		{
			return EngineCore::GetInstance();
		}
	}
}
