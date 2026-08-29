#version 410 core

layout(location = 0) in vec4 vp;
layout(location = 1) in vec4 normal_in;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix; 

out vec3 position;
out vec3 normal;

void main () {

  gl_Position = modelViewProjectionMatrix * vp;
  position = (modelViewMatrix * vp).xyz;
  normal = (modelViewMatrix * normal_in).xyz;
}
  
