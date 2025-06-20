#version 440

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 2, binding = 0) uniform _perObject {
	mat4 mvp;
	mat4 modelMatrix;
} perObject;

/////////////////
// VertexInput //
/////////////////
layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

///////////////////
// FragmentInput //
///////////////////
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outWorldPos;
layout (location = 2) out vec2 outTexCoord;
layout (location = 3) out vec3 outNormal;

void main(void) {
    outColor = inColor;
	outWorldPos = vec3(perObject.modelMatrix * inPos);
    outTexCoord = inTexCoord;
	
	outNormal = normalize(mat3(perObject.modelMatrix) * inNormal);
	
    gl_Position = perObject.mvp * inPos;
}
