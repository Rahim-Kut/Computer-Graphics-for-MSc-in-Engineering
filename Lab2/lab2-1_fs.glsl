// Build on your code from Lab 1 here
#version 410 core

out vec3 frag_colour;

in vec3 position;
in vec3 normal;

void main(){
    frag_colour = normalize(normal) * 0.5 +0.5;
}

