#pragma once
#pragma warning(disable:4251)

#include "Base\BaseTypes.h"
#include "BasePlatform\BasePlatformLogWriter.h"

namespace cube
{
	namespace core
	{
		class EngineCore;
		class InputManager;

		class Renderer3D;
		enum class RenderType;
		class RendererManager;

		class Mesh;

		class Material;

		class ModuleManager;

		class GameObject;
	}
}

#ifdef ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS