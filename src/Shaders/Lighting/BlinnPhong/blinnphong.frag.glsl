#version 330

#define MAX_LIGHTS 8

// attributs interpolés à partir des valeurs en sortie du shader de sommet
in vec3 surface_position;
in vec3 surface_normal;
in vec3 light_position[MAX_LIGHTS];

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// facteur de brillance spéculaire du matériau
uniform float brightness;
uniform int nbLights;


void main()
{
  // re-normaliser la normale après interpolation
  vec3 n = normalize(surface_normal);
  fragment_color = vec4(vec3(0), 1);

  for(int i = 0; i < nbLights; i++) {
    // calculer la direction de la surface vers la lumière (l)
    vec3 l = normalize(light_position[i] - surface_position);

    // calculer le niveau de réflexion diffuse (n • l)
    float reflection_diffuse = max(dot(n, l), 0.0);

    // réflexion spéculaire par défaut
    float reflection_specular = 0.0;

    // calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
    if (reflection_diffuse > 0.0)
    {
      // calculer la direction de la surface vers la caméra (v)
      vec3 v = normalize(-surface_position);

      // calculer la direction du demi-vecteur de réflection (h) en fonction du vecteur de vue (v) et de lumière (l)
      vec3 h = normalize(v + l);

      // calculer le niveau de réflexion spéculaire (n • h)
      reflection_specular = pow(max(dot(n, h), 0.0), brightness);
    }

    // calculer la couleur du fragment
    fragment_color += vec4(
      color_ambient +
      color_diffuse * reflection_diffuse +
      color_specular * reflection_specular, 0.0);
  }
}