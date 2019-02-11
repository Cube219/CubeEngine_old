#version 440

////////////
// Global //
////////////
layout (set = 0, binding = 0) uniform _global {
	vec3 cameraPos;
} global;

layout (set = 0, binding = 1) uniform _dirLight {
	int isExisted;
	vec4 color;
	vec3 direction;
} dirLight;

layout (set = 0, binding = 2) uniform _pointLights {
	int num;
	vec4 color[10];
	vec3 position[10];
} pointLights;

/////////////////
// PerMaterial //
/////////////////
layout (set = 1, binding = 0) uniform _perMaterial {
	vec4 albedo;
	float roughness;
	float metallic;
} perMaterial;
layout (set = 1, binding = 1) uniform sampler2D texSampler;

///////////////////
// FragmentInput //
///////////////////
layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 inWorldPos;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inNormal;

////////////////////
// FragmentOutput //
////////////////////
layout (location = 0) out vec4 outColor;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnaelSchlick(float cosT, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosT, 5.0);
}

vec3 BRDF(vec3 L, vec3 V, vec3 N, vec3 albedo, float metallic, float roughness)
{
	vec3 H = normalize(V + L);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	vec3 F = fresnaelSchlick(clamp(dot(N, V), 0.0, 1.0), F0);
	
	float D = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	
	vec3 nom = D * G * F;
	float denom = 4.0 * clamp(dot(N, V), 0.0, 1.0) * clamp(dot(N, L), 0.0, 1.0);
	
	vec3 specular = nom / max(denom, 0.00001);
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	
	kD *= 1.0 - metallic;
	
	vec3 Lo = (kD * (albedo / PI) + specular);

	return Lo;
}

void main(void) {
    vec4 textureColor = texture(texSampler, inTexCoord);
	
	// Light
	vec3 lightColor = vec3(0.0);
	
	vec3 N = normalize(inNormal);
	vec3 V = normalize(global.cameraPos - inWorldPos);
	
	// Dir light
	if(dirLight.isExisted == 1) {
		vec3 L = normalize(-dirLight.direction);

		vec3 Lo = BRDF(L, V, N, vec3(perMaterial.albedo), perMaterial.metallic, perMaterial.roughness);
	
		float NdotL = max(dot(N, L), 0.0);
		Lo *= vec3(dirLight.color) * NdotL;

		lightColor += Lo;
	}

	// Point lights
	for(int i = 0; i < pointLights.num; i++) {
		vec3 L = normalize(pointLights.position[i] - inWorldPos);

		vec3 Lo = BRDF(L, V, N, vec3(perMaterial.albedo), perMaterial.metallic, perMaterial.roughness);

		float distance = length(pointLights.position[i] - inWorldPos);
		float attenuation = 1.0 / (distance * distance);
		float cosT = max(dot(N, L), 0.0);
		vec3 radiance = vec3(pointLights.color[i]) * attenuation * cosT;

		float NdotL = max(dot(N, L), 0.0);
		Lo *= radiance * NdotL;

		lightColor += Lo;
	}

	vec3 ambient = vec3(0.03) * vec3(perMaterial.albedo);
	vec3 color = ambient + lightColor;

	//color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	
	outColor = vec4(color, 1.0);
}
