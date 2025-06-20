#version 440

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 1, binding = 0) uniform samplerCube skybox;

layout (location = 0) in vec3 inTexCoord;

layout (location = 0) out vec4 outColor;

void main(void) {
    outColor = texture(skybox, inTexCoord);
}
