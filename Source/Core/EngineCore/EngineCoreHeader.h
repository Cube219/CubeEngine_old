#pragma once
#pragma warning(disable:4251)

#include "Base/BaseTypes.h"
#include "Assertion.h"

namespace cube
{
	namespace core
	{
		class EngineCore;
		
		class ThreadManager;
		class Thread;

		class TimeManager;
		class GameTime;

		class ResourceManager;
		class ResourceImporter;
		class Resource;
		template <typename T>
		class ResourcePointer;

		class LogWriter;

		class BasicString;
		class PathString;
		class ImmutableString;
		class StringManager;

		template <typename T>
		struct BasicHandlerData;
		template <typename T>
		class BasicHandler;

		class Renderer3D;
		class CameraRenderer3D;
		enum class RenderType;
		class RendererManager;

		struct Vertex;
		class Mesh;

		class BaseMeshGenerator;

		enum class MaterialParameterType;
		struct MaterialParameterInfo;
		struct MaterialInitializer;
		class Material;
		using MaterialData = BasicHandlerData<Material>;
		using HMaterial = BasicHandler<Material>;
		class MaterialInstance;
		using MaterialInstanceData = BasicHandlerData<MaterialInstance>;
		using HMaterialInstance = BasicHandler<MaterialInstance>;

		class Shader;

		class BaseLight;
		class DirectionalLight;
		class PointLight;
		class SpotLight;

		class Texture;

		class ModuleManager;

		class GameObjectManager;
		class GameObject;
		using GameObjectData = BasicHandlerData<GameObject>;
		using HGameObject = BasicHandler<GameObject>;

		class ComponentManager;
		class Component;
		using ComponentData = BasicHandlerData<Component>;
		using HComponent = BasicHandler<Component>;
	}
}

#ifdef ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS

#include "Resource/ResourcePointer.h"
