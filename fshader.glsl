#version 130

in vec4 color;
in vec3 normal;
in vec2 tex;
in float usetex;

in vec4 cameraPos;
in vec4 eye;

in float l1;
in float l2;

out vec4 frag_color;

uniform sampler2D texture;
uniform mat4 uViewMatrix;

void main () 
{
  vec4 COLOR = vec4(0.0);
  vec4 diffuse = vec4(0.45, 0.45, 0.45, 1.0);
  vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
  vec4 specular = vec4(0.2, 0.2, 0.2, 1.0);
  float shininess = 10.0;
  vec4 spec = vec4(0.5);
  vec3 n = normalize(vec3(normal));  
  vec4 texImage = color;

  if(usetex > 0)
  {
     texImage = texture2D(texture, tex);
  }

  if(l1 > 0)
  {
      // Defining Light 
      vec4 lightPos = vec4(1.0, 8.0, 0.0, 0.0);
      vec3 lightDir = vec3(uViewMatrix * lightPos);  // Transforms with camera
      lightDir = normalize( vec3(lightDir));  

      //Diffuse    
      float dotProduct = dot(n, lightDir);
      float intensity =  max( dotProduct, 0.0);

      // Compute specular component only if light falls on vertex
      if(intensity > 0.0)
      {
      vec3 e = normalize(vec3(eye));
      vec3 h = normalize(lightDir + e );
        float intSpec = max(dot(h,n), 0.0); 
            spec = specular * pow(intSpec, shininess);
      }  

      COLOR = max(intensity * diffuse  + spec, ambient)*texImage; // All
      
  }

  if(l2 > 0)
  {
      vec4 spotlightPos = uViewMatrix * vec4(-23.0, 26.8, -23.0, 1.0);
      vec3 spotlightDir = vec3(spotlightPos - cameraPos);
      float dotProduct = dot(n,normalize(spotlightDir));
      float intensity =  max( dotProduct, 0.0);

      vec3 spotDirection = vec3((uViewMatrix * vec4(0.0, -5.0, 0, 1.0)) - spotlightPos);

      if(intensity > 0.2) {
          vec3 e = normalize(vec3(eye));
          vec3 h = normalize(spotlightDir + e );
          float spotEffect = dot(normalize(spotDirection), normalize(-spotlightDir));
          if(spotEffect > 0.9) {
              COLOR += spotEffect * (diffuse * intensity + ambient) * texImage;
              float intSpec = max(dot(h,n), 0.0);
              spec = specular * pow(intSpec, shininess);
              COLOR += spotEffect * spec * texImage;
          }
      }
  }
  

  frag_color = COLOR;

}
