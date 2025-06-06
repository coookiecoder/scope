#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    if (fragColor.r != 1.0 || fragColor.g != 1.0 || fragColor.b != 1.0) {
        outColor = vec4(fragColor, 1.0);
    } else {
        outColor = texture(texSampler, fragTexCoord);
    }
}
