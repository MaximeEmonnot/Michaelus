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

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;
layout(location = 3) out vec2 fragTexCoord;

void main(){
    vec4 positionWorldSpace = ubo.model * vec4(inPosition, 1.0);
    gl_Position = ubo.projection * ubo.view * positionWorldSpace;
    fragNormalWorld = normalize(mat3(ubo.model) * inNormal);
    fragPosWorld = positionWorldSpace.xyz;
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}