#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D tex0;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
void main() {
	FragColor = texture(tex0, texCoord) * color;
}