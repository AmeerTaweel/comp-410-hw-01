#version 330 core

in vec3 pos;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(pos, 1.0);
}
