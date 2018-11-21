#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexcord;

uniform float vUseTexture;
uniform float uLight1;
uniform float uLight2;


out vec4 color;
out vec3 normal;
out vec2 tex;
out float usetex;




out vec4 cameraPos;
out vec4 eye;
out float l1;
out float l2;


uniform mat4 uModelViewMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uViewMatrix;

void main (void) 
{
  cameraPos = uModelViewMatrix * vPosition;
  gl_Position = uModelViewMatrix * vPosition;
  normal = (uNormalMatrix * normalize(vNormal));
  eye = - (uModelViewMatrix * gl_Position);
  color = vColor;
  tex = vTexcord;
  normal = vNormal;
  usetex = vUseTexture;

  l1 = uLight1;
  l2 = uLight2;
}
