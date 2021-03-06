// Shader inspired from https://gist.github.com/Hebali/6ebfc66106459aacee6a9fac029d0115

#version 330

uniform sampler2D tex0;
uniform vec2 texSize;
uniform float threshold;

in vec2 texCoordVarying;

out vec4 outputColor;

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord) {
	float w = 1.0 / texSize.x;
	float h = 1.0 / texSize.y;

	n[0] = texture(tex, coord + vec2(-w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(w, -h));
	n[3] = texture(tex, coord + vec2(-w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(w, 0.0));
	n[6] = texture(tex, coord + vec2(-w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(w, h));
}

void main() {
	vec4 n[9];
	make_kernel(n, tex0, texCoordVarying);

	vec4 sobel_edge_h = n[2] + (threshold * n[5]) + n[8] - (n[0] + (threshold * n[3]) + n[6]);
	vec4 sobel_edge_v = n[0] + (threshold * n[1]) + n[2] - (n[6] + (threshold * n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

	outputColor = vec4(sobel.rgb, texture(tex0, texCoordVarying).a);
}
