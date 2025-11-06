#version 450
out vec3 frag_colour;
in vec3 position;

void main () {
  frag_colour = vec3(position * 0.5 + 0.5);
}