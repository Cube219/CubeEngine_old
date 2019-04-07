#version 440

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 0) uniform _global {
	vec3 cameraPos;
	mat4 viewProj;
} global;

/////////////////
// VertexInput //
/////////////////
layout (location = 0) in vec4 inPos;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec2 inTexCoord;

layout (location = 0) out vec3 outTexCoord;

void main(void) {
    outTexCoord = vec3(inPos.xyz);
	
	gl_Position = global.viewProj * (inPos + vec4(global.cameraPos, 0));
}
