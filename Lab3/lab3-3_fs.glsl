#version 410 core

out vec3 frag_colour;

in vec3 position;
in vec3 normal;
in vec2 uv_coords;
in vec3 tangent;
in vec3 bitangent;

uniform sampler2D tex_sampler;
uniform samplerCube env_sampler;
uniform sampler2D normal_sampler;

void main(){
    vec3 T = normalize(tangent);
    vec3 B = normalize(bitangent);
    vec3 N = normalize(normal);

    mat3 TBN = mat3(T, B, N);

    vec3 sampled_normal = texture(normal_sampler, uv_coords).rgb;
    sampled_normal = sampled_normal * 2.0 - 1.0;

    vec3 mapped_normal = normalize(TBN * sampled_normal);

    vec3 V = normalize(position);
    vec3 R = reflect(V, mapped_normal);

    vec3 tex_colour = texture(tex_sampler, uv_coords).rgb;
    vec3 env_colour = texture(env_sampler, R).rgb;

    vec3 die_tint = vec3(0.95, 0.95, 0.55);
    vec3 base_colour = tex_colour * die_tint;
    
    float reflectiveness = 0.45;
    frag_colour = mix(base_colour, env_colour, reflectiveness);
}