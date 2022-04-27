#version 330

#define MAX_LIGHTS 8

in vec2 texcoord;

// attributs de sommet
in vec4 position;
in vec4 normal;

// attributs en sortie
out vec2 texCoordVarying;
out vec3 surface_position;
out vec3 surface_normal;
out vec3 light_position[MAX_LIGHTS];

// attributs uniformes
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;
uniform mat4   textureMatrix;

uniform vec3 lightPos[MAX_LIGHTS];
uniform int	 nbLights;

void main() {
	// calculer la matrice normale
	mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

	// transformation de la normale du sommet dans l'espace de vue
	surface_normal = vec3(normalMatrix * normal);

	// transformation de la position du sommet dans l'espace de vue
	surface_position = vec3(modelViewMatrix * position);

	for (int i = 0; i < nbLights; i++)
		light_position[i] = vec3(modelViewMatrix * vec4(lightPos[i], 1.0));

	texCoordVarying = texcoord;

	// transformation de la position du sommet par les matrices de modèle, vue et projection
	gl_Position = projectionMatrix * modelViewMatrix * position;
}