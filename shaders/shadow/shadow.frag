#version 330 core
layout(location = 0) out float aPos;

void main() {
  aPos = gl_FragCoord.z;
}