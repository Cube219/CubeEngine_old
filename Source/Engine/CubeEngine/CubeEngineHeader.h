#pragma once
#pragma warning(disable:4251)

#include "Base/BaseTypes.h"

namespace cube
{
	class CubeEngine;

	class CameraComponent;
	class MoveComponent;
}

#ifdef CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllexport) 
#else // CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllimport) 
#endif // CUBE_ENGINE_EXPORTS
