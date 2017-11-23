#pragma once

#include <glm.hpp>

namespace cube
{
	namespace core
	{
		struct Vertex
		{
			glm::vec4 pos;      // Position data
			glm::vec4 color;    // Color
			glm::vec3 normal;   // Normal
			glm::vec2 texCoord; // Texture coordination
		};
	}
}