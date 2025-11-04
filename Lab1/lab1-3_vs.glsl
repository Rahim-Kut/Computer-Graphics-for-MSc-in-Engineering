#version 450
in vec3 vp;

void main () {
  
	// Change this so that you don't just set `gl_Position` to `vp`, but
	// also add an offset from a `uniform` variable:
	gl_Position = vec4(vp, 1.0);

};
  
