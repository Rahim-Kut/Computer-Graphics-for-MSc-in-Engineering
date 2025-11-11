#version 450
out vec3 frag_colour;
in vec4 position;

void main () {
	frag_colour = vec3(position.z * 0.5 + 0.5);

}
