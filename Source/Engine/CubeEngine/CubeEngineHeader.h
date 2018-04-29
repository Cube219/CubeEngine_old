#pragma once
#pragma warning(disable:4251)

#include "Base/BaseTypes.h"

namespace cube
{
	class CubeEngine;

	class ShaderImporter;
	class TextureImporter;
	class ObjImporter;

	class CameraComponent;
	class MoveComponent;
	class Renderer3DComponent;
	class DirectionalLightComponent;
	class PointLightComponent;
}

#ifdef CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllexport) 
#else // CUBE_ENGINE_EXPORTS
	#define CUBE_ENGINE_EXPORT __declspec(dllimport) 
#endif // CUBE_ENGINE_EXPORTS
