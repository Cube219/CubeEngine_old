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

		class InputManager;
		
		class TimeManager;
		class GameTime;

		class LogWriter;

		class BasicString;
		class PathString;
		class ImmutableString;
		class StringManager;

		class Renderer3D;
		class CameraRenderer3D;
		enum class RenderType;
		class RendererManager;

		class Mesh;

		class Material;

		class Texture;

		class ModuleManager;

		class GameObject;
	}
}

#ifdef ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllexport) 
#else // ENGINE_CORE_EXPORTS
#define ENGINE_CORE_EXPORT __declspec(dllimport) 
#endif // ENGINE_CORE_EXPORTS