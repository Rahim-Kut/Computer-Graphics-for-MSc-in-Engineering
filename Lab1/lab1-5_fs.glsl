#version 450
out vec3 frag_colour;
in vec4 position;

void main () {

	// Set color proportional to the z position here, instead of a constant. 
	frag_colour = vec3(0.5, position.z * 0.5, 0.5);
}
