#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec3 texCoord;

out vec4 color;
out vec3 Normal;
out vec3 tex;

uniform mat4 uModelViewMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  color = vColor;
  tex = texCoord;
  Normal = vNormal;
}
