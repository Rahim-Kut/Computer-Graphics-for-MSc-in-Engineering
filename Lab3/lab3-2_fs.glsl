#version 410 core

out vec3 frag_colour;

in vec3 position;
in vec3 normal;
in vec2 uv_coords;

uniform sampler2D tex_sampler;
uniform samplerCube env_sampler;

void main(){

    vec3 N = normalize(normal);

    vec3 V = normalize(position);
    vec3 R = reflect(V, N);

    vec3 tex_colour = texture(tex_sampler, uv_coords).rgb;
    vec3 env_colour = texture(env_sampler, R).rgb;

    vec3 die_tint = vec3(0.95, 0.95, 0.55);
    vec3 base_colour = tex_colour * die_tint;
    
    float reflectiveness = 0.45;
    frag_colour = mix(base_colour, env_colour, reflectiveness);
}