#define ESCAPE 27

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include "primitive.hpp"

// Camera position and rotation Parameters
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 6.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0,c_yrot=0.0,c_zrot=0.0;

//Running variable to toggle culling on/off
bool enable_culling=true;

//Enable/Disable perspective view
bool enable_perspective=false;

//Shader program attribs
GLuint vPosition,vColor,vNormal,texCoord;

//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

cs475::Model model1;

cs475::Model model2;

cs475::Model model3;

cs475::Model model4;


cs475::Sphere* sphere = new cs475::Sphere();
cs475::Cone* cone = new cs475::Cone();
cs475::Cuboid* cuboid = new cs475::Cuboid();
cs475::Disc* disc = new cs475::Disc();
//cs475::Trapezoid* trapezoid = new cs475::Trapezoid();