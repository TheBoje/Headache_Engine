#version 330

uniform vec4 globalColor;

out vec4 outputColor;

void main() {
	outputColor = globalColor;
}