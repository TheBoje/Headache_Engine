#version 330
precision highp float;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

out VertexAttrib {
	vec3 texcoord;
}
vertex;

void main(void) {
	vertex.texcoord = normalize(position.xyz);
	gl_Position = modelViewProjectionMatrix * position;
}