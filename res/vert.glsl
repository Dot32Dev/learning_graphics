#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform vec2 scale;
uniform vec2 position;

void main() {
    gl_Position = vec4((aPos.xy + position)/scale, aPos.z, 1.0);
	// gl_Position = vec4(aPos.xy/scale, aPos.z, 1.0);
	vertexColor = gl_Position;
	TexCoord = aTexCoord;
}