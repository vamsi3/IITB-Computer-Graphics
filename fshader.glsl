#version 130

in vec4 color;
in vec3 normal;
in vec2 tex;
in float usetex;

out vec4 frag_color;

uniform sampler2D texture;

void main () 
{
  vec4 COLOR = vec4(0.0);

  if(usetex < 0)
  {
   	 COLOR = color;
  }
  else
  {
  	 COLOR = texture2D(texture, tex);	
  }
  
  frag_color = COLOR;
}
