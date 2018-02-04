#version 440

layout (set = 0, binding = 0) uniform sampler2D texSampler;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec4 outColor;

void main(void) {
    outColor = texture(texSampler, texCoord);
}