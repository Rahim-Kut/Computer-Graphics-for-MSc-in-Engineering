#version 450
layout(location=0) in vec3 vp;
out vec3 position;
uniform vec2 position_offset;

void main () {
	gl_Position = vec4(vp + vec3(position_offset, 0.0) , 1.0);
	// position = vp;  static colour
	position = vp + vec3(position_offset, 0.0); // dynamic colour
 };
  
