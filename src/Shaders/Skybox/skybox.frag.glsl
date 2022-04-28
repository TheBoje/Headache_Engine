#version 330
precision highp float;

uniform samplerCube EnvMap;

in VertexAttrib {
	vec3 texcoord;
}
vertex;

out vec4 fragColor;

void main(void) {
	vec4 envColor = texture(EnvMap, vertex.texcoord);
	fragColor = envColor;
}