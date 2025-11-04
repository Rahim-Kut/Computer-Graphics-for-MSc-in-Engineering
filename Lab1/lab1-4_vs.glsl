#version 450
in vec3 vp;

void main () {
  gl_Position = vec4 (vec3(vp), 1.0);
};
  
