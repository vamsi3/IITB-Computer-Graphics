#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexcord;

uniform float vUseTexture;

out vec4 color;
out vec3 normal;
out vec2 tex;
out float usetex;

uniform mat4 uModelViewMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  color = vColor;
  tex = vTexcord;
  normal = vNormal;
  usetex = vUseTexture;
}
