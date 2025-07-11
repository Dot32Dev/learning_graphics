#version 410 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform vec2 scale;

void main() {
    gl_Position = vec4(aPos.xy/scale, aPos.z, 1.0);
	vertexColor = gl_Position;
}