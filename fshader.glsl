#version 130

in vec4 color;
in vec3 Normal;
in vec3 tex;

out vec4 frag_color;

uniform sampler2D texture;

void main () 
{
  vec4 COLOR = vec4(0.0);

  float texNeed = tex.x;

  vec2 tex1 = vec2(tex.y, tex.z);
  
  if(texNeed < 0)
  {
   	 COLOR = color;
  }
  else
  {
  	 COLOR = texture2D(texture, tex1);	
  }
  
  frag_color = COLOR;
}
