#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

vec3 getNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

// Move the face upon -50 unit in normal's direction.
// Note that the face will not be display if the normal is not in
// the camera direction.
vec4 explode(vec4 position, vec3 normal) {
	float magnitude = 2.0;
	vec3  direction = normal * (-50);
	return position + vec4(direction, 0.0);
}

void main() {
	vec3 normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal);
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	EmitVertex();
	EndPrimitive();
}