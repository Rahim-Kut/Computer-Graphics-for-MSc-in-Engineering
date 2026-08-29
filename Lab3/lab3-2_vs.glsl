#version 410 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

out vec3 position;
out vec3 normal;
out vec2 uv_coords;

void main(){
    vec4 vertex_position = vec4(vp, 1.0);

    gl_Position = modelViewProjectionMatrix * vertex_position;
    position = (modelViewMatrix * vertex_position).xyz;
    normal = normalize(mat3(modelViewMatrix) * normal_in);

    uv_coords = texcoord;
}