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
    DirectionalLight directional;
    vec4 ambientColor;
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

    // DIRECTIONNAL
    vec3 directionnalLightColor = ubo.directional.color.xyz * ubo.directional.color.w;
    vec3 directionnalLighting = directionnalLightColor * max(dot(normalize(fragNormalWorld), ubo.directional.direction.xyz), 0.0);

    // POINT LIGHTS
	vec3 diffusePointLighting = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < ubo.numLights; i++){
    	vec3 directionToPointLight = ubo.pointLights[i].position.xyz - fragPosWorld;
    	float attenuation = 1.0 / dot(directionToPointLight, directionToPointLight);
    	vec3 pointLightColor = ubo.pointLights[i].color.xyz * ubo.pointLights[i].color.w * attenuation;
    	diffusePointLighting += pointLightColor * max(dot(normalize(fragNormalWorld), normalize(directionToPointLight)), 0.0);
	}

    // AMBIENT
    vec3 ambientLighting = ubo.ambientColor.xyz * ubo.ambientColor.w;

	outColor = vec4((directionnalLighting + diffusePointLighting + ambientLighting) * fragColor, 1.0) * texture(texSampler, fragTexCoord);
}