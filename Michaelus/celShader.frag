#version 450

struct DirectionalLight {
    vec4 direction;
    vec4 color;
};

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
	vec4 cameraPosition;
	vec4 lightIntensities;
    DirectionalLight directional;
    PointLight pointLights[32];
    int numLights;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main(){

	// CONSTANTS
	const float ambientIntensity = ubo.lightIntensities.x;
	const float diffuseIntensity = ubo.lightIntensities.y;
	const float lightDetails = 1.0 / ubo.lightIntensities.z;
	//const float lightDetails = 1.0 / ubo.lightIntensities.w;

    // DIRECTIONNAL
    vec3 directionalLightColor = ubo.directional.color.xyz * ubo.directional.color.w;
	float directionalNoL = dot(normalize(fragNormalWorld), normalize(ubo.directional.direction.xyz));
    vec3 directionalLighting = directionalLightColor * diffuseIntensity * floor(max(directionalNoL, 0.0) / lightDetails);

    // POINT LIGHTS
	vec3 pointLighting = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < ubo.numLights; i++){
    	vec3 directionToPointLight = ubo.pointLights[i].position.xyz - fragPosWorld;
    	float attenuation = 1.0 / dot(directionToPointLight, directionToPointLight);
    	vec3 pointLightColor = ubo.pointLights[i].color.xyz * ubo.pointLights[i].color.w * attenuation;
		float pointNoL = dot(normalize(fragNormalWorld), normalize(directionToPointLight));
    	pointLighting += pointLightColor * (diffuseIntensity * floor(max(pointNoL, 0.0) / lightDetails));
	}

    // AMBIENT
    vec3 ambientLighting = fragColor * ambientIntensity;

	outColor = vec4((ambientLighting + directionalLighting + pointLighting) * fragColor, 1.0) * texture(texSampler, fragTexCoord);
}