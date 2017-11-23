#version 440

layout (binding = 1) uniform sampler2D texSampler;

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texCoord;

layout (location = 0) out vec4 outColor;

void ComputeDirectionalLight()
{
}

void ComputePointLight()
{
}

void ComputeSpotLight()
{
}

void main(void) {
    outColor = texture(texSampler, texCoord);
}