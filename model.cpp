#include "model.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;

GLuint uModelViewMatrix;


//-----------------------------------------------------------------

const int num_vertices = 1000000;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];

//-----------------------------------------------------------------


void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("vshader.glsl");
  std::string fragment_shader_file("fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(cs475::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(cs475::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = cs475::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");


  //--------------------------Model1-------------------------------//

  

  // partial_sphere->set(2.0, -M_PI/2, M_PI/2, 0, M_PI/2, 40)->setColor(1,0,0,1)->load(v_positions, v_colors);
  // partial_cone->set(3.0, 3.0, 0.0, 2.0, 0, M_PI, 0.1, 36)->load(v_positions, v_colors);
  // partial_disc->set(2.0, 0.0, M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  // cs475::Quad quad; quad.setColor(glm::vec4(1,1,1,1)); quad.set(glm::vec4(0,0,0,1),glm::vec4(1,0,0,1),glm::vec4(1,1,0,1),glm::vec4(0,1,0,1)); quad.load(v_positions, v_colors, 0);
  
  //tnode->change_parameters(0.0,0.0,0.0,180,0.0,0.0);

  double s=1.8;


  //LowerTorso
  cone->set(1.2*s, 1.0*s, 0, 2.0*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  cs475::HNode* tnode = new cs475::HNode(NULL,cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-6,-8,0,90,0,0);
  (model1.nodes).push_back(tnode);



  //UpperTorso
  cone->set(1.2*s, 0.5*s, 0, 0.5*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[0],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0,-180,0,0);
  (model1.nodes).push_back(tnode);


  //Neck
  sphere->set(0.5*s, 0.5*s, 0.25*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[1],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.5*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //Head
  sphere->set(0.7*s, 0.7*s, 0.7*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[2],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.95*s,0,0,0);
  (model1.nodes).push_back(tnode);
  

  //RShoulder
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[0],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(1.4*s,0,0,0,30,0);
  (model1.nodes).push_back(tnode);


  //LShoulder
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[0],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-1.4*s,0,0,0,-30,0);
  (model1.nodes).push_back(tnode);


  //RUpperArm
  cone->set(0.2*s, 0.2*s, 0, 1.0*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[4],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.1*s,0,0,0,90,0);
  (model1.nodes).push_back(tnode);


  //LUpperArm
  cone->set(0.2*s, 0.2*s, 0, 1.0*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[5],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-0.1*s,0,0,0,-90,0);
  (model1.nodes).push_back(tnode);


  //RElbow
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[6],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,1.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LElbow
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[7],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,1.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLowerArm
  cone->set(0.2*s, 0.1*s, 0, 0.8*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[8],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LLowerArm
  cone->set(0.2*s, 0.1*s, 0, 0.8*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[9],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //Hip
  cone->set(1.0*s, 1.0*s, 0, 0.5*s, 0, 2*M_PI, 360)->setColor(0.6,0.6,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[0],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,2.0*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLegShoulder
  sphere->set(0.3*s, 0.3*s, 0.3*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[12],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.65*s,0,0.6*s,0,10,0);
  (model1.nodes).push_back(tnode);


  //LLegShoulder
  sphere->set(0.3*s, 0.3*s, 0.3*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[12],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-0.65*s,0,0.6*s,0,-10,0);
  (model1.nodes).push_back(tnode);


  //RUpperLeg
  cone->set(0.3*s, 0.2*s, 0, 1.2*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[13],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,20,0);
  (model1.nodes).push_back(tnode);


  //LUpperLeg
  cone->set(0.3*s, 0.2*s, 0, 1.2*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[14],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,-20,0);
  (model1.nodes).push_back(tnode);


  //RKnee
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[15],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,1.2*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LKnee
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[16],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,1.2*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLowerLeg
  cone->set(0.2*s, 0.15*s, 0, 1.0*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[17],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LLowerLeg
  cone->set(0.2*s, 0.15*s, 0, 1.0*s, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[18],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //nose
  sphere->set(0.1*s, 0.15*s, 0.15*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,0,0,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[3],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,-0.7*s,0,0,0,0);
  (model1.nodes).push_back(tnode);


  //discforhip
  disc->set(1.0*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model1.nodes[12],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.5*s,0,0,0);
  (model1.nodes).push_back(tnode);



  //hatbase
  cone->set(1.5*s, 1.5*s, 0, 0.1*s, 0, 2*M_PI, 360)->setColor(0,0.6,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[3],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.4*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //discsforhatbase
  disc->set(1.5*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model1.nodes[23],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0,0,0,0);
  (model1.nodes).push_back(tnode);


  disc->set(1.5*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model1.nodes[23],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //hat
  cone->set(0.65*s, 0.65*s, 0, 0.75*s, 0, 2*M_PI, 360)->setColor(0,0.6,1,1)->load(v_positions, v_colors);
  
  tnode = new cs475::HNode(model1.nodes[23],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //discforhat
  disc->set(0.65*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model1.nodes[26],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,0.75*s,0,0,0);
  (model1.nodes).push_back(tnode);



  model1.curr_node = model1.nodes[0];
  model1.limb = 0;





  //---------------------------Model2---------------------------------//

  cuboid->set(0.0, 0.0, -0.0, 0, -0.0, 0.0)->setColor(1.0,1.0,0,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,3,0,10,30,0);
  (model2.nodes).push_back(tnode);
  

  cuboid->set(-6.0, 6.0, -5.0, 5, -0.1, 0.1)->setColor(1.0,1.0,0,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model2.nodes[0],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,10,-6.0,0,0,0);
  (model2.nodes).push_back(tnode);
  
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,0,12.0,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(-0.1, 0.1, -5.0, 5, -5.9, 5.9)->setColor(1,0,1,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-5.9,0,6.0,0,0,0);
  (model2.nodes).push_back(tnode);
  
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(5.9,0,6.0,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(-6, 6, -0.1, 0.1, -6.1, 6.1)->setColor(0,1,1,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,-5.1,6.0,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(12, 0, 0.2, 0, 12.2, 0)->setColor(0,1,1,1)->load(v_positions, v_colors, 1);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-6,5,-0.1,0,0,0);
  (model2.nodes).push_back(tnode);
  

  model2.curr_node = model2.nodes[0];
  model2.limb = 0;

  

  //--------------------------Model3-------------------------------//

  double s3=0.15;

  // main-body - 0
  int m3_0_r=10, m3_0_half_h=15;
  double m3_rot_body=20;
  cone->set(m3_0_r*s3, m3_0_r*s3, -m3_0_half_h*s3, m3_0_half_h*s3, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  tnode = new cs475::HNode(NULL,cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(6.0,-8.0,0.0,-(90+m3_rot_body),15.0,-30.0);
  (model3.nodes).push_back(tnode);

  // head - 1
  sphere->set(m3_0_r*s3, m3_0_r*s3, m3_0_r*s3, 0, M_PI, 0, 2*M_PI, 360, true)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[0],sphere->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,m3_0_half_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // botton-main-rod - 2
  int m3_2_half_w=2, m3_2_h=4;
  cuboid->set(-m3_2_half_w*s3, m3_2_half_w*s3, -m3_2_half_w*s3, m3_2_half_w*s3, m3_2_h*s3, -m3_2_h*s3)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model3.nodes[0],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,-m3_0_half_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // botton-main-feet - 3
  int m3_3_half_w1=2, m3_3_half_w2=7, m3_3_h=8;
  double m3_rot=50;
  trapezoid->set(m3_3_half_w1*s3, m3_3_half_w2*s3, m3_3_half_w1*s3, m3_3_half_w2*s3, 0, -m3_3_h*s3)->load(v_positions, v_colors, 1);
  tnode = new cs475::HNode(model3.nodes[2],trapezoid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,-m3_2_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand Elbow - 4
  int m3_4_r=5, m3_4_half_h=3;
  cone->set(m3_4_r*s3, m3_4_r*s3, -m3_4_half_h*s3, m3_4_half_h*s3, 0, M_PI, 360)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[0],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(m3_0_r*s3,0,(m3_0_half_h-m3_4_r)*s3,90,90,m3_rot);
  (model3.nodes).push_back(tnode);

  // 5
  tnode = new cs475::HNode(model3.nodes[0],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(-m3_0_r*s3,0,(m3_0_half_h-m3_4_r)*s3,90,90,m3_rot);
  (model3.nodes).push_back(tnode);

  // Hand Arm - 6
  int m3_5_half_h=16;
  cuboid->set(-m3_4_r*s3, m3_4_r*s3, 0, 2*m3_5_half_h*s3, 0, m3_4_half_h*s3)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model3.nodes[4],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,-2*m3_5_half_h*s3,0,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // 7
  cuboid->set(-m3_4_r*s3, m3_4_r*s3, 0, 2*m3_5_half_h*s3, -m3_4_half_h*s3, 0)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors, 0);
  tnode = new cs475::HNode(model3.nodes[5],cuboid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,-2*m3_5_half_h*s3,0,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand End - 8, 9

  trapezoid->set(m3_4_r*s3, 1.7*m3_4_r*s3, (m3_4_half_h*s3)/2, (2.5*m3_4_half_h*s3)/2, 0, 5*s3)->load(v_positions, v_colors, 1);
  tnode = new cs475::HNode(model3.nodes[6],trapezoid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,(m3_4_half_h*s3)/2,90,0,0.0);
  (model3.nodes).push_back(tnode);

  trapezoid->set(m3_4_r*s3, 1.7*m3_4_r*s3, (m3_4_half_h*s3)/2, (2.5*m3_4_half_h*s3)/2, 0, 5*s3)->load(v_positions, v_colors, 1);
  tnode = new cs475::HNode(model3.nodes[7],trapezoid->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,-(m3_4_half_h*s3)/2,90,0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand Discs - 10, 11

  disc->set(m3_4_r*s3,0,M_PI,360)->setColor(0.26,0.52,0.89,1)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[4],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,m3_4_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  tnode = new cs475::HNode(model3.nodes[5],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,-m3_4_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  // Base Disc - 12

  disc->set(m3_0_r*s3,0,2*M_PI,360)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[0],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,-m3_0_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  // Eye - 13

  cone->set(2*s3, 2*s3, -m3_4_half_h*s3, (m3_4_half_h*0.75)*s3, 0, 2*M_PI, 360)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[1],cone->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0,-10*s3,2.5*s3,80,0,0);
  (model3.nodes).push_back(tnode);

  // 14
  disc->set(2*s3,0,2*M_PI,360)->load(v_positions, v_colors);
  tnode = new cs475::HNode(model3.nodes[13],disc->getVertexCount(),v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  tnode->change_parameters(0.0,0.0,m3_4_half_h*0.75*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);  


  model3.curr_node = model3.nodes[0];
  model3.limb = 0;


}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-20.0, 20.0, -20.0, 20.0, 1.0, 30.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);

  model1.nodes[0]->render_tree();
  model2.nodes[0]->render_tree();
  model3.nodes[0]->render_tree();
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(cs475::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(800, 800, "Assignment-2 Modelling", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, cs475::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, cs475::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  cs475::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

