#pragma once
#pragma warning(disable:4251)

#include "Base/BaseTypes.h"

namespace cube
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

	class BasicString;
	class PathString;
	class ImmutableString;
	class StringManager;

	template <typename T>
	class Handler;
	class Handlable;
	class HandlerTable;

	class RenderObject;

	class Renderer3D;
	using HRenderer3D = Handler<Renderer3D>;
	class CameraRenderer3D;
	enum class RenderType;
	class RendererManager;

	struct Vertex;
	class MeshData;
	class Mesh;

	class BaseMeshGenerator;

	enum class MaterialParameterType;
	struct MaterialParameterInfo;
	class Material;
	using HMaterial = Handler<Material>;
	class MaterialInstance;
	using HMaterialInstance = Handler<MaterialInstance>;
		
	class Shader;

	class BaseLight;
	class DirectionalLight;
	using HDirectionalLight = Handler<DirectionalLight>;
	class PointLight;
	using HPointLight = Handler<PointLight>;
	class SpotLight;

	class TextureData;
	class Texture;

	class Skybox;
	using HSkybox = Handler<Skybox>;

	class ModuleManager;
	class BaseModule;

	class GameObjectManager;
	class GameObject;
	using HGameObject = Handler<GameObject>;

	class ComponentManager;
	class Component;
	using HComponent = Handler<Component>;

	namespace rt
	{
		class Renderer3D;
		class CameraRenderer3D;

		class RenderObject;

		class Material;
		class MaterialInstance;

		class BaseLight;
		class DirectionalLight;
		class PointLight;

		class Mesh;

		class Texture;

		class Skybox;
	} // namespace rt
} // namespace cube


#ifdef ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS

#include "Resource/ResourcePointer.h"
