#version 450

in vec4 vp;
uniform mat4 uMVP;
out vec4 position;
void main () {

	gl_Position = uMVP * vp;
    position = gl_Position;

}