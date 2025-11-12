#version 450

in vec4 vp;
uniform mat4 modelViewProjectionMatrix;
out vec4 position;
void main () {
	
	//-------------------------------------------------------------------------//
	// Instead of passing the `vp` variable unaffected as below, 
	// apply the model, view and projection transform to vertex
	// positions here. Forward the position to the fragment shader using
	// an appropriate `out` variable.
	// -------------------------------------------------------------------------//

	gl_Position = modelViewProjectionMatrix * vp;
	position = gl_Position;

}