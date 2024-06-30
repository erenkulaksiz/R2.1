#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoord = aTex;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Tangent = normalize(mat3(transpose(inverse(model))) * aTangent);
    Bitangent = normalize(mat3(transpose(inverse(model))) * aBitangent);
    TBN = mat3(Tangent, Bitangent, Normal);
}