#include "EngineCore.h"

#include "Time/TimeManager.h"
#include "Time/GameTime.h"
#include "String/StringManager.h"
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
#include "Renderer/Renderer3D.h"
#include "Renderer/CameraRenderer3D.h"

namespace cube
{
	namespace core
	{
		EngineCore* EngineCore::mInstance = nullptr;

		EngineCore::EngineCore(SPtr<platform::BasePlatform>& platform) :
			mPlatform(platform), mFPSLimit(-1)
		{
			platform->SetLoopFunction(std::bind(&EngineCore::Loop, this));
			platform->SetResizeFunction(std::bind(&EngineCore::Resize, this, _1, _2));
		}

		EngineCore::~EngineCore()
		{
		}

		void EngineCore::Prepare()
		{
			LogWriter::Init(mPlatform);

			mTimeManager = std::make_unique<TimeManager>();
			mStringManager = std::make_unique<StringManager>();

			mRendererManager = std::make_unique<RendererManager>(mPlatform, RenderType::Vulkan);

			mResourceManager = std::make_unique<ResourceManager>(mPlatform->GetFileSystem());
			mResourceManager->RegisterImporter(
				std::make_unique<TextureImporter>(mRendererManager->GetRenderAPI())
			);
			mResourceManager->RegisterImporter(
				std::make_unique<ShaderImporter>(mRendererManager->GetRenderAPI())
			);

			mThreadManager = std::make_shared<ThreadManager>();

			mModuleManager = std::make_unique<ModuleManager>(mThreadManager);

			// Create mesh / texture
			mBoxMesh = BaseMeshGenerator::GetBoxMesh();

			WString texturePath = L"../../../SampleResources/Textures/TestTexture.png";
			mTexture = mResourceManager->LoadResource<Texture>(texturePath);
			texturePath = L"../../../SampleResources/Textures/TestTexture2.png";
			mTexture2 = mResourceManager->LoadResource<Texture>(texturePath);

			// Load shader
			WString shaderPath = L"../../../SampleResources/Shaders/Vertex.glsl";
			mMaterialVertexShader = mResourceManager->LoadResource<Shader>(shaderPath);

			shaderPath = L"../../../SampleResources/Shaders/Fragment.glsl";
			mMaterialFragmentShader = mResourceManager->LoadResource<Shader>(shaderPath);

			// Create material
			MaterialInitializer matInit;
			matInit.shaders.push_back(mMaterialVertexShader);
			matInit.shaders.push_back(mMaterialFragmentShader);
			matInit.parameters.push_back({"Texture", MaterialParameterType::Texture, 0});
			mMaterial = std::make_shared<Material>(mRendererManager->GetRenderAPI(), matInit);
			mRendererManager->RegisterMaterial(mMaterial);

			// Create materialInstances
			mMaterialIns1 = mMaterial->CreateInstance();
			String t = "Texture";
			mMaterialIns1->SetParameterData<RPtr<Texture>>(t, mTexture);

			mMaterialIns2 = mMaterial->CreateInstance();
			mMaterialIns2->SetParameterData<RPtr<Texture>>(t, mTexture2);

			int flag = -1;
			// Create gameobjects
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					for(int k = -1; k <= 1; k++) {

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
		}

		void EngineCore::Run()
		{
			mTimeManager->Start();

			mPlatform->StartLoop();
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

			for(auto& go : mGos) {
				go->Update();
			}

			mRendererManager->GetCameraRenderer3D()->RotateTemp(mTimeManager->GetGlobalGameTime()->GetDeltaTime());
			
			mRendererManager->DrawAll();

			// Limit FPS
			if(mFPSLimit > 0) {
				double nextTime = currentTime + (1.0 / mFPSLimit);

				currentTime = mTimeManager->GetSystemTime();

				double waitTime = nextTime - currentTime;

				if(waitTime > 0.1) { // TODO: 적절한 수치를 찾기
					mPlatform->Sleep(SCast(int)(waitTime * 1000));
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