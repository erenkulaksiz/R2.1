#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
out vec2 texCoord;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 billboardPos;
uniform vec3 camPos;

void main() {
	vec3 viewDir = normalize(camPos - billboardPos);
	vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), viewDir));
	vec3 up = cross(viewDir, right);

	mat4 billboardMatrix = mat4(vec4(right, 0.0), vec4(up, 0.0), vec4(viewDir, 0.0), vec4(billboardPos, 1.0));

	gl_Position = projection * view * billboardMatrix * vec4(aPos, 1.0);

	FragPos = aPos;
	texCoord = aTex;
}
