#pragma once

#include "Base/Vector.h"

namespace cube
{
	struct Vertex
	{
		Vector4 pos;      // Position data
		Vector4 color;    // Color
		Vector3 normal;   // Normal
		Vector2 texCoord; // Texture coordination
	};
} // namespace cube
