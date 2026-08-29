#version 410

out vec3 frag_colour;

in vec3 normal;
in vec3 position;

uniform int light_count;
uniform vec3 light_position[4];
uniform vec3 light_colour[4];

const float PI = 3.14159265359;

vec3 blinn_phong_brdf(vec3 in_direction, vec3 out_direction, vec3 normal){
    float kL = 0.8;
    float kg = 0.2;
    float s = 80.0;

    vec3 L = vec3(0.8, 0.5, 0.3);
    vec3 g = vec3(1.0, 1.0, 1.0);

    vec3 half_vector = normalize(in_direction + out_direction);

    vec3 diffuse = kL * L / PI;
    vec3 glossy = kg * g * ((s + 8.0) / (8.0 * PI)) * pow(max(dot(normal, half_vector), 0.0), s);

    return diffuse + glossy;
}

void main(){
    frag_colour = vec3(0.0);

    vec3 n = normalize(normal);
    vec3 out_direction = normalize(-position);

    for (int l = 0; l < light_count; ++l){
        vec3 in_direction = normalize(light_position[l] - position);
        float cosine = max(dot(in_direction, n), 0.0);

        vec3 brdf = blinn_phong_brdf(in_direction, out_direction, n);

        frag_colour += light_colour[l] * brdf * cosine;
    }
}
