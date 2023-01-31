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