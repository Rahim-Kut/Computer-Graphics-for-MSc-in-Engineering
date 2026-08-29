#version 410

out vec3 frag_colour;

in vec3 normal;
in vec3 position;
in vec3 axis;

uniform int light_count;
uniform vec3 light_position[4];
uniform vec3 light_colour[4];

const float PI = 3.14159265359;
const float EPSILON = 0.0001;

vec3 fresnel_schlick(vec3 F0, float wo_dot_h){
    return F0 + (vec3(1.0) - F0) * pow(1.0 - wo_dot_h, 5.0);
}

float anisotropic_beckmann_distribution(vec3 n, vec3 h, vec3 tangent, vec3 bitangent, float alpha_tangent, float alpha_bitangent){
    float n_dot_h = max(dot(n, h), 0.0);

    if (n_dot_h <= 0.0){
        return 0.0;
    }

    float t_dot_h = dot(tangent, h);
    float b_dot_h = dot(bitangent, h);

    float alpha_t2 = alpha_tangent * alpha_tangent;
    float alpha_b2 = alpha_bitangent *
    alpha_bitangent;

    float n_dot_h2 = n_dot_h * n_dot_h;
    float n_dot_h4 = n_dot_h2 * n_dot_h2;

    float exponent = -((t_dot_h * t_dot_h) / alpha_t2 + (b_dot_h * b_dot_h) / alpha_b2) / n_dot_h2;

    return exp(exponent) / (PI * alpha_tangent * alpha_bitangent * n_dot_h4);
}


float cook_geometry(float n_dot_h, float n_dot_wi, float n_dot_wo, float wo_dot_h){
    if (wo_dot_h <= 0.0){
        return 0.0;
    }

    float g1 = 1.0;
    float g2 = (2.0 * n_dot_h * n_dot_wo) / wo_dot_h;
    float g3 = (2.0 * n_dot_h * n_dot_wi) / wo_dot_h;

    return min(g1, min(g2, g3));
}

vec3 cook_torrance_brdf(vec3 in_direction, vec3 out_direction, vec3 normal){
    normal = normalize(normal);
    vec3 a = normalize(axis);
    vec3 bitangent = normalize(cross(normal, a));

    if (length(bitangent) < EPSILON){
        bitangent = normalize(cross(normal, vec3(1.0, 0.0, 0.0)));
    }

    vec3 tangent = normalize(cross(bitangent, normal));

    float kL = 0.2;
    float kg = 0.8;

    vec3 rho = vec3(1.0, 0.78, 0.34);
    vec3 F0 = vec3(1.0, 0.78, 0.34);

    //float alpha_b = 0.3;
    float alpha_tangent = 0.7;
    float alpha_bitangent = 0.15;

    vec3 h = normalize(in_direction + out_direction);

    float n_dot_wi = max(dot(normal, in_direction), 0.0);
    float n_dot_wo = max(dot(normal, out_direction), 0.0);
    float n_dot_h = max(dot(normal, h), 0.0);
    float wo_dot_h = max(dot(out_direction, h), 0.0);

    vec3 diffuse = rho / PI;

    vec3 F = fresnel_schlick(F0, wo_dot_h);
    float D = anisotropic_beckmann_distribution(normal, h, tangent, bitangent, alpha_tangent, alpha_bitangent);
    float G = cook_geometry(n_dot_h, n_dot_wi, n_dot_wo, wo_dot_h);

    vec3 specular = (F * D * G) / max(4.0 * n_dot_wi * n_dot_wo, EPSILON);

    return kL * diffuse + kg * specular;
}

void main(){
    frag_colour = vec3(0.0);

    vec3 n = normalize(normal);
    vec3 out_direction = normalize(-position);

    for (int l = 0; l < light_count; ++l){
        vec3 in_direction =
        normalize(light_position[l] - position);
        float cosine = max(dot(in_direction, n), 0.0);

        vec3 brdf = cook_torrance_brdf(in_direction, out_direction, n);

        frag_colour += light_colour[l] * brdf * cosine;
    }
}
