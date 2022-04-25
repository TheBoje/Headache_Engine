#version 330

#define MAX_LIGHTS 8

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;
in vec3 light_position[MAX_LIGHTS];

in vec2 texCoordVarying;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform int nbLights;

uniform sampler2D tex0;

// facteur de brillance spéculaire du matériau
uniform float brightness;

// position d'une source de lumière

void main()
{
  // à terme utiliser la specular map, diffuse map etc...
  vec3 ca = (color_ambient * texture(tex0, texCoordVarying).rgb);
  vec3 cd = (color_diffuse * texture(tex0, texCoordVarying).rgb);
  vec3 cs = (color_specular * texture(tex0, texCoordVarying).rgb);

  vec3 n = normalize(surface_normal);
  vec3 v = normalize(-surface_position);

  vec4 colorSum = vec4(0);

  for(int i = 0; i < nbLights; i++) {
    vec3 l = normalize(light_position[i] - surface_position);
    
    float reflection_diffuse = max(dot(n, l), 0.0);
    float reflection_specular = 0.0;
    
    if (reflection_diffuse > 0.0)
    {
      // calculer la direction de la réflection (r) du rayon incident (-l) en fonction de la normale (n)
      vec3 r = reflect(-l, n);

      // calculer le niveau de réflexion spéculaire (r • v)
      reflection_specular = pow(max(dot(v, r), 0.0), brightness);
    }

    colorSum += vec4(cd * reflection_diffuse + cs * reflection_specular, 0.0);
  }

  fragment_color = vec4(ca, 1) + colorSum;
}