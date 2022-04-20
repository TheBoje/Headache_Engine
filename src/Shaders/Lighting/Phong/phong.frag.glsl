#version 330

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;
in vec3 light_position;  

// in vec2 texCoordVarying;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// uniform sampler2D tex0;

// facteur de brillance spéculaire du matériau
uniform float brightness;

// position d'une source de lumière

void main()
{
  // à terme utiliser la specular map, diffuse map etc...
  vec3 ca = color_ambient;//(color_ambient * texture(tex0, texCoordVarying).rgb);
  vec3 cd = color_diffuse;//(color_diffuse * texture(tex0, texCoordVarying).rgb);
  vec3 cs = color_specular;//(color_specular * texture(tex0, texCoordVarying).rgb);

  // re-normaliser la normale après interpolation
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumière (l)
  vec3 l = normalize(light_position - surface_position);

  // calculer le niveau de réflexion diffuse (n • l)
  float reflection_diffuse = max(dot(n, l), 0.0);

  // réflexion spéculaire par défaut
  float reflection_specular = 0.0;

  // calculer la réflexion spéculaire seulement s'il y a réflexion diffuse
  if (reflection_diffuse > 0.0)
  {
    // calculer la direction de la surface vers la caméra (v)
    vec3 v = normalize(-surface_position);

    // calculer la direction de la réflection (r) du rayon incident (-l) en fonction de la normale (n)
    vec3 r = reflect(-l, n);

    // calculer le niveau de réflexion spéculaire (r • v)
    reflection_specular = pow(max(dot(v, r), 0.0), brightness);
  }

  // calculer la couleur du fragment
  fragment_color = vec4(
    ca +
    cd * reflection_diffuse +
    cs * reflection_specular, 1.0);
}