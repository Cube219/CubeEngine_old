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
		class MaterialInstance;

		class Shader;

		class BaseLight;
		class DirectionalLight;
		class PointLight;
		class SpotLight;

		class Texture;

		class ModuleManager;

		class GameObject;

		class ComponentManager;
		class Component;
	}
}

#ifdef ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
	#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS

#include "Resource/ResourcePointer.h"
