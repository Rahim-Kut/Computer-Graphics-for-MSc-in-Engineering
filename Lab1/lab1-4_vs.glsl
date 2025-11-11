#version 450
in vec3 vp;
out vec4 position;

void main () {
  gl_Position = vec4(vec3(vp), 1.0);
  position = gl_Position;
};
  
