#pragma once

#include "Base/BaseTypes.h"

namespace cube
{
	class CubeEngine;

	class MoveComponent;
}

#ifdef CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllexport) 
#else // CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllimport) 
#endif // CUBE_ENGINE_EXPORTS
