// Shader inspired from https://openframeworks.cc/ofBook/chapters/shaders.html#blurring

#version 330

uniform sampler2D tex0;
uniform float blurAmnt;
uniform vec2 texSize;

in vec2 texCoordVarying;
out vec4 outputColor;

void main() {
	vec4 color;

	float w = blurAmnt / texSize.x;
	float h = blurAmnt / texSize.y;

	// Horizontal blur
	color += 1.0 * texture(tex0, texCoordVarying + vec2(w * -4.0, 0.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(w * -3.0, 0.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(w * -2.0, 0.0));
	color += 4.0 * texture(tex0, texCoordVarying + vec2(w * -1.0, 0.0));

	color += 5.0 * texture(tex0, texCoordVarying + vec2(w, 0));

	color += 4.0 * texture(tex0, texCoordVarying + vec2(w * 1.0, 0.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(w * 2.0, 0.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(w * 3.0, 0.0));
	color += 1.0 * texture(tex0, texCoordVarying + vec2(w * 4.0, 0.0));

	// Vertical blur
	color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, h * -4.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, h * -3.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, h * -2.0));
	color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, h * -1.0));

	color += 5.0 * texture(tex0, texCoordVarying + vec2(0, h));

	color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, h * 1.0));
	color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, h * 2.0));
	color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, h * 3.0));
	color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, h * 4.0));

	color /= 50.0;

	outputColor = color;
}