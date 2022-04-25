#version 330

#define MAX_LIGHTS 8

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;
in vec3 light_position[MAX_LIGHTS];

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;

uniform int nbLights;

void main()
{
  // re-normaliser la normale après interpolation (n)
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumière (l)
  
  float reflection_diffuse = 0.0;
  for(int i = 0; i < nbLights; i++) {
    vec3 l = normalize(light_position[i] - surface_position);

    // calculer le niveau de réflexion diffuse (n • l)
    reflection_diffuse += max(dot(n, l), 0.0);
  }

  // déterminer la couleur du fragment
  fragment_color = vec4(color_ambient + color_diffuse * reflection_diffuse, 1.0);
}