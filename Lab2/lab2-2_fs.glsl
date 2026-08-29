#version 410

out vec3 frag_colour;

in vec3 normal;
in vec3 position;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

const float PI = 3.14159265359;


vec3 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  vec3 albedo = vec3(1.0, 0.82, 0.35);
  return albedo/PI;
}

void main () {

  frag_colour = vec3(0);
  vec3 n = normalize(normal);
  vec3 out_direction = normalize(-position);

  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
    vec3 in_direction = normalize(light_position[l] - position);
    float cosine = max(dot(in_direction, n), 0.0);
    vec3 brdf = lambertian_brdf(in_direction, out_direction, n);

    frag_colour += light_colour[l] * brdf * cosine;
  }
}
