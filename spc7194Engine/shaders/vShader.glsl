#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

layout (location = 3) uniform mat4 modelWorldMatrix;
layout (location = 4) uniform mat4 cameraMat;

out vec2 FragUV;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = cameraMat * modelWorldMatrix * vec4(position, 1);
	FragUV = uv;
	FragPos = vec3(modelWorldMatrix * vec4(position, 1));
	Normal = mat3(transpose(inverse(modelWorldMatrix))) * normal;
}