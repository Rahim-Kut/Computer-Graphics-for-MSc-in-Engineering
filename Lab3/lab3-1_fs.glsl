#version 410 core

out vec3 frag_colour;

in vec3 position;
in vec3 normal;
in vec2 uv_coords;

uniform sampler2D tex_sampler;

void main(){
    
    vec3 N = normalize(normal);

    // light direction in view space
    vec3 L = normalize(vec3(0.3, 0.8, 1.0));

    float diffuse = max(dot(N, L), 0.0);
    vec3 tex = texture(tex_sampler, uv_coords).rgb;

    // yellow die frag_colour
    vec3 die_tint = vec3(0.95, 0.95, 0.55);
    vec3 albedo = tex * die_tint;
    float ambient = 0.25;
    frag_colour = albedo * (ambient + diffuse);

    //vec4 tex = texture(tex_sampler, uv_coords);
    //frag_colour = tex.rgb;
}
