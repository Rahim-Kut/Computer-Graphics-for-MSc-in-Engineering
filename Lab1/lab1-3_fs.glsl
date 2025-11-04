#version 400
out vec3 frag_colour;

void main () {

	// Change this to use input from a `uniform` variable instead of just a constant color:
	frag_colour = vec3(0.5, 0.0, 0.5); 
}
