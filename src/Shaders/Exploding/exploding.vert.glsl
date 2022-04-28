#version 330

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

void main() {
	gl_Position = modelViewProjectionMatrix * position;
}