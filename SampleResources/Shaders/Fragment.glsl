#version 440

////////////
// Global //
////////////
layout (set = 0, binding = 0) uniform _dirLight {
	vec4 diffuse;
	vec3 direction;
} dirLight;

/////////////////
// PerMaterial //
/////////////////
layout (set = 1, binding = 0) uniform sampler2D texSampler;

///////////////////
// FragmentInput //
///////////////////
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

////////////////////
// FragmentOutput //
////////////////////
layout (location = 0) out vec4 outColor;

void main(void) {
    vec4 textureColor = texture(texSampler, inTexCoord);
	
	vec3 lightDir = -dirLight.direction;
	float lightIntensity = clamp(dot(inNormal, lightDir), 0.0, 1.0);
	vec4 lightColor = clamp(dirLight.diffuse * lightIntensity, vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0));
	
	outColor = textureColor * lightColor;
}
