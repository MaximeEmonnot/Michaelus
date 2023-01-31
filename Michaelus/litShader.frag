#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec4 directionnalLight;
    vec4 directionnalColor;
    vec4 ambientColor;
    vec4 pointLight;
    vec4 pointColor;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

void main(){

    // DIRECTIONNAL
    vec3 directionnalLightColor = ubo.directionnalColor.xyz * ubo.directionnalColor.w;
    vec3 directionnalLighting = directionnalLightColor * max(dot(normalize(fragNormalWorld), ubo.directionnalLight.xyz), 0.0);

    // POINT LIGHTS
    vec3 directionToPointLight = ubo.pointLight.xyz - fragPosWorld;
    float attenuation = 1.0 / dot(directionToPointLight, directionToPointLight);
    vec3 pointLightColor = ubo.pointColor.xyz * ubo.pointColor.w * attenuation;
    vec3 diffusePointLighting = pointLightColor * max(dot(normalize(fragNormalWorld), normalize(directionToPointLight)), 0.0);

    // AMBIENT
    vec3 ambientLighting = ubo.ambientColor.xyz * ubo.ambientColor.w;

	outColor = vec4((directionnalLighting + diffusePointLighting + ambientLighting) * fragColor, 1.0) * texture(texSampler, fragTexCoord);
}