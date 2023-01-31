#version 450

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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main(){
    // POSITION
    vec4 positionWorldSpace = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.projection * ubo.view * positionWorldSpace;

    // NORMAL
    vec3 normalWorldSpace = normalize(mat3(ubo.model) * inNormal);

    // DIRECTIONNAL
    vec3 directionnalLightColor = ubo.directionnalColor.xyz * ubo.directionnalColor.w;
    vec3 directionnalLighting = directionnalLightColor * max(dot(normalWorldSpace, ubo.directionnalLight.xyz), 0.0);

    // POINT LIGHTS
    vec3 directionToPointLight = ubo.pointLight.xyz - positionWorldSpace.xyz;
    float attenuation = 1.0 / dot(directionToPointLight, directionToPointLight);
    vec3 pointLightColor = ubo.pointColor.xyz * ubo.pointColor.w * attenuation;
    vec3 diffusePointLighting = pointLightColor * max(dot(normalWorldSpace, normalize(directionToPointLight)), 0.0);

    // AMBIENT
    vec3 ambientLighting = ubo.ambientColor.xyz * ubo.ambientColor.w;

    fragColor = (directionnalLighting + diffusePointLighting + ambientLighting) * inColor;

    fragTexCoord = inTexCoord;
}