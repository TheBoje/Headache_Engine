#version 330

uniform sampler2D tex0;

in vec2 texCoordVarying;

out vec4 outputColor;

float average(vec3 color) {
	return (color.r + color.g + color.b) / 3;
}

void main() {
	outputColor = vec4(vec3(average(texture(tex0, texCoordVarying).rgb)), texture(tex0, texCoordVarying).a);
}