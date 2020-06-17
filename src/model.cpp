#include "model.hpp"
#include "texture.hpp"

GLuint shaderProgram;


GLuint uModelViewMatrix,uViewMatrix,uNormalMatrix;

unsigned char *animation_frame;


//-----------------------------------------------------------------

glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];
glm::vec3 v_normals[num_vertices];
glm::vec2 v_texcord[num_vertices];

//-----------------------------------------------------------------

void drawSphere(glm::vec3 center)
{
  sphere->set(0.7, 0.7, 0.7, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,1,1)->load();
  
  cs475::HNode* tnode = new cs475::HNode(model12.nodes[0],sphere->getVertexCount());
  if(center.z > 0)
  {
    tnode->change_parameters(center.x,center.y,center.z-1,0,0,0);  
  }
  else
  {
    tnode->change_parameters(center.x,center.y,center.z+1,0,0,0);
  }
  
}

std::string itos(int k) {
  return std::to_string(k) + " ";
}

std::string itos(double k) {
  return std::to_string(k) + " ";
}

std::string itos(bool k) {
  return std::to_string(k) + " ";
}

double stodb(std::string s) {
  return std::stod(s);
}

void dumpFrame() {
  std::ofstream frame_records;
  frame_records.open("keyframes.txt", std::ios::app);
  frame_records << itos(light1) + itos(light2);

  for(auto k: model1.nodes) {
    frame_records << itos(k->tx) + itos(k->ty) + itos(k->tz) + itos(k->rx) + itos(k->ry) + itos(k->rz);
  }

for(auto k: model2.nodes) {
    frame_records << itos(k->tx) + itos(k->ty) + itos(k->tz) + itos(k->rx) + itos(k->ry) + itos(k->rz);
  }


  for(auto k: model3.nodes) {
    frame_records << itos(k->tx) + itos(k->ty) + itos(k->tz) + itos(k->rx) + itos(k->ry) + itos(k->rz);
  }


  for(auto k: model4.nodes) {
    frame_records << itos(k->tx) + itos(k->ty) + itos(k->tz) + itos(k->rx) + itos(k->ry) + itos(k->rz);
  }
  
  frame_records << itos(c_xpos) + itos(c_ypos) + itos(c_zpos) + itos(c_xrot) + itos(c_yrot) + itos(c_zrot);
  
  frame_records << "\n";

  frame_records.close();
}

std::vector<double> parse_frame(std::string s) {
  std::vector<double> frame;
  std::istringstream is(s);
  std::string part;
  while(getline(is, part, ' ')) {
    frame.push_back(stodb(part));
  }
  return frame;
}

std::vector<std::vector<double>> interpolate_two_frames(int t){
  std::vector<std::vector<double>> i_frames;

  for(int i=0;i<24;i++)
  {
    std::vector<double> frame;
    for(int j=0;j<keyframes[t].size();j++){
      frame.push_back((keyframes[t][j]*(24-i)+keyframes[t+1][j]*i)/24);
    }
    
    i_frames.push_back(frame);
    frame.clear();
  }
  
  return i_frames;
}


void interpolate_all_frames(){
  for(int i=0;i<keyframes.size()-1;i++){
    std::vector<std::vector<double>> frames = interpolate_two_frames(i);
    allframes.insert(allframes.end(),frames.begin(),frames.end());
  }
  allframes.push_back(keyframes[keyframes.size()-1]);
}



void readKeyframes() {

  keyframes.clear();
  std::string frame;
  std::ifstream frame_records("keyframes.txt");
  if(frame_records.is_open()) {
    while(getline(frame_records, frame)) {
      keyframes.push_back(parse_frame(frame));
    }
    frame_records.close();
    interpolate_all_frames();
    mode = 3;
  }
  else {
    std::cout << "Unable to open keyframes.txt file";
  }
}



void applyFrame(int kf_num) {
  std::vector<double> frame = allframes[kf_num];
  int j = 0;
  light1 = frame[j++] > 0.0 ? 1 : -1;
  light2 = frame[j++] > 0.0 ? 1 : -1;
  
  for(auto k: model1.nodes) {
    k->tx = frame[j++];
    k->ty = frame[j++];
    k->tz = frame[j++];
    k->rx = frame[j++];
    k->ry = frame[j++];
    k->rz = frame[j++];
    k->update_matrices();
  }

for(auto k: model2.nodes) {
    k->tx = frame[j++];
    k->ty = frame[j++];
    k->tz = frame[j++];
    k->rx = frame[j++];
    k->ry = frame[j++];
    k->rz = frame[j++];
    k->update_matrices();
  }



  for(auto k: model3.nodes) {
    k->tx = frame[j++];
    k->ty = frame[j++];
    k->tz = frame[j++];
    k->rx = frame[j++];
    k->ry = frame[j++];
    k->rz = frame[j++];
    k->update_matrices();
  }


  for(auto k: model4.nodes) {
    k->tx = frame[j++];
    k->ty = frame[j++];
    k->tz = frame[j++];
    k->rx = frame[j++];
    k->ry = frame[j++];
    k->rz = frame[j++];
    k->update_matrices();
  }
  
  c_xpos = frame[j++];
  c_ypos = frame[j++];
  c_zpos = frame[j++];
  c_xrot = frame[j++];
  c_yrot = frame[j++];
  c_zrot = frame[j++];
}

void captureFrame(unsigned int animation_framenum)
{
  animation_frame = new unsigned char [3 * (screen_width+1) * (screen_height + 1) ];


  glReadBuffer(GL_BACK);

  glPixelStoref(GL_PACK_ALIGNMENT,1);
  glReadPixels(0, 0, screen_width, screen_height, GL_RGB, GL_UNSIGNED_BYTE, animation_frame);
  char filename[200];
  sprintf(filename,"frames/frame_%04d.ppm",animation_framenum);
  std::ofstream out(filename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<screen_width<<" "<<screen_height<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(animation_frame), (3 * (screen_width+1) * (screen_height + 1)) * sizeof(int));
  out.close();

  delete animation_frame;
}


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
  vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
  vTexcord = glGetAttribLocation( shaderProgram, "vTexcord" );
  vUseTexture = glGetUniformLocation( shaderProgram, "vUseTexture");
 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  uNormalMatrix = glGetUniformLocation( shaderProgram, "uNormalMatrix");
  uViewMatrix = glGetUniformLocation( shaderProgram, "uViewMatrix");

  uLight1 = glGetUniformLocation( shaderProgram, "uLight1");
  uLight2 = glGetUniformLocation( shaderProgram, "uLight2");

  glUniform1f(uLight1,light1);
  glUniform1f(uLight2,light2);

   //-------------------------- Model 1 (JOKER) -------------------------------//

  

  // partial_sphere->set(2.0, -M_PI/2, M_PI/2, 0, M_PI/2, 40)->setColor(1,0,0,1)->load(v_positions, v_colors);
  // partial_cone->set(3.0, 3.0, 0.0, 2.0, 0, M_PI, 0.1, 36)->load(v_positions, v_colors);
  // partial_disc->set(2.0, 0.0, M_PI, 36)->setColor(0, 1, 0, 1)->load(v_positions, v_colors);
  // cs475::Quad quad; quad.setColor(glm::vec4(1,1,1,1)); quad.set(glm::vec4(0,0,0,1),glm::vec4(1,0,0,1),glm::vec4(1,1,0,1),glm::vec4(0,1,0,1)); quad.load(v_positions, v_colors, 0);
  
  //tnode->change_parameters(0.0,0.0,0.0,180,0.0,0.0);

  
  double s=0.5;
  double s2 = 0.6;
  double s3=0.06;
  double s4 = 6;
  double s5 = 1;
  double s6 = 1;
  double s7 = 1;
  double s8 = 1;
  double s9 = 1;
  double s10 = 1;
  double s11 = 1;

  double m2_l = 12.4*s2, m2_b = 12*s2, m2_h = 10.4*s2;
  double m4_l = 10*s4, m4_b = 10*s4, m4_h = 8*s4;

  GLuint wooden = LoadTexture("images/wooden.bmp",512,512);
  GLuint floor = LoadTexture("images/all1.bmp",256,256);
  GLuint bricks = LoadTexture("images/bricks.bmp",638,425);
  GLuint carpet = LoadTexture("images/carpet.bmp",499,337);
  GLuint marble = LoadTexture("images/marble.bmp",400,400);
  GLuint metal = LoadTexture("images/metal.bmp",300,375);
  GLuint roof = LoadTexture("images/roof.bmp",600,400);
  GLuint stones = LoadTexture("images/stones.bmp",638,494);
  GLuint suit2 = LoadTexture("images/suit2.bmp",358,358);
  GLuint wood = LoadTexture("images/wood.bmp",512,512);  


  //LowerTorso
  cone->set(1.2*s, 1.0*s, 0, 2.0*s, 0, 2*M_PI, 360)->load();
  
  cs475::HNode* tnode = new cs475::HNode(NULL,cone->getVertexCount());
  tnode->change_parameters(-m2_l/4,0,0,90,0,0);
  (model1.nodes).push_back(tnode);



  //UpperTorso
  cone->set(1.2*s, 0.5*s, 0, 0.5*s, 0, 2*M_PI, 360)->load();
  
  tnode = new cs475::HNode(model1.nodes[0],cone->getVertexCount());
  tnode->change_parameters(0,0,0,-180,0,0);
  (model1.nodes).push_back(tnode);


  //Neck
  sphere->set(0.5*s, 0.5*s, 0.25*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[1],sphere->getVertexCount());
  tnode->change_parameters(0,0,0.5*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //Head
  sphere->set(0.7*s, 0.7*s, 0.7*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[2],sphere->getVertexCount());
  tnode->change_parameters(0,0,0.95*s,0,0,0);
  (model1.nodes).push_back(tnode);
  

  //RShoulder
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[0],sphere->getVertexCount());
  tnode->change_parameters(1.4*s,0,0,0,30,0);
  (model1.nodes).push_back(tnode);


  //LShoulder
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[0],sphere->getVertexCount());
  tnode->change_parameters(-1.4*s,0,0,0,-30,0);
  (model1.nodes).push_back(tnode);


  //RUpperArm
  cone->set(0.2*s, 0.2*s, 0, 1.0*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[4],cone->getVertexCount());
  tnode->change_parameters(0.1*s,0,0,0,90,0);
  (model1.nodes).push_back(tnode);


  //LUpperArm
  cone->set(0.2*s, 0.2*s, 0, 1.0*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[5],cone->getVertexCount());
  tnode->change_parameters(-0.1*s,0,0,0,-90,0);
  (model1.nodes).push_back(tnode);


  //RElbow
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[6],sphere->getVertexCount());
  tnode->change_parameters(0,0,1.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LElbow
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[7],sphere->getVertexCount());
  tnode->change_parameters(0,0,1.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLowerArm
  cone->set(0.2*s, 0.1*s, 0, 0.8*s, 0, 2*M_PI, 360)->load();
  
  tnode = new cs475::HNode(model1.nodes[8],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LLowerArm
  cone->set(0.2*s, 0.1*s, 0, 0.8*s, 0, 2*M_PI, 360)->load();
  
  tnode = new cs475::HNode(model1.nodes[9],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //Hip
  cone->set(1.0*s, 1.0*s, 0, 0.5*s, 0, 2*M_PI, 360)->setColor(0.6,0.6,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[0],cone->getVertexCount());
  tnode->change_parameters(0,0,2.0*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLegShoulder
  sphere->set(0.3*s, 0.3*s, 0.3*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[12],sphere->getVertexCount());
  tnode->change_parameters(0.65*s,0,0.6*s,0,10,0);
  (model1.nodes).push_back(tnode);


  //LLegShoulder
  sphere->set(0.3*s, 0.3*s, 0.3*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(0,1,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[12],sphere->getVertexCount());
  tnode->change_parameters(-0.65*s,0,0.6*s,0,-10,0);
  (model1.nodes).push_back(tnode);


  //RUpperLeg
  cone->set(0.3*s, 0.2*s, 0, 1.2*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[13],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,20,0);
  (model1.nodes).push_back(tnode);


  //LUpperLeg
  cone->set(0.3*s, 0.2*s, 0, 1.2*s, 0, 2*M_PI, 360)->setColor(1,0,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[14],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,-20,0);
  (model1.nodes).push_back(tnode);


  //RKnee
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[15],sphere->getVertexCount());
  tnode->change_parameters(0,0,1.2*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LKnee
  sphere->set(0.2*s, 0.2*s, 0.2*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,1,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[16],sphere->getVertexCount());
  tnode->change_parameters(0,0,1.2*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //RLowerLeg
  cone->set(0.2*s, 0.15*s, 0, 1.0*s, 0, 2*M_PI, 360)->load();
  
  tnode = new cs475::HNode(model1.nodes[17],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //LLowerLeg
  cone->set(0.2*s, 0.15*s, 0, 1.0*s, 0, 2*M_PI, 360)->load();
  
  tnode = new cs475::HNode(model1.nodes[18],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //nose
  sphere->set(0.1*s, 0.15*s, 0.15*s, 0, 2*M_PI, 0, 2*M_PI, 360, true)->setColor(1,0,0,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[3],sphere->getVertexCount());
  tnode->change_parameters(0,-0.7*s,0,0,0,0);
  (model1.nodes).push_back(tnode);


  //discforhip
  disc->set(1.0*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load();
  tnode = new cs475::HNode(model1.nodes[12],disc->getVertexCount());
  tnode->change_parameters(0,0,0.5*s,0,0,0);
  (model1.nodes).push_back(tnode);



  //hatbase
  cone->set(1.5*s, 1.5*s, 0, 0.1*s, 0, 2*M_PI, 360)->setColor(0,0.6,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[3],cone->getVertexCount());
  tnode->change_parameters(0,0,0.4*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //discsforhatbase
  disc->set(1.5*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load();
  tnode = new cs475::HNode(model1.nodes[23],disc->getVertexCount());
  tnode->change_parameters(0,0,0,0,0,0);
  (model1.nodes).push_back(tnode);


  disc->set(1.5*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load();
  tnode = new cs475::HNode(model1.nodes[23],disc->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //hat
  cone->set(0.65*s, 0.65*s, 0, 0.75*s, 0, 2*M_PI, 360)->setColor(0,0.6,1,1)->load();
  
  tnode = new cs475::HNode(model1.nodes[23],cone->getVertexCount());
  tnode->change_parameters(0,0,0.1*s,0,0,0);
  (model1.nodes).push_back(tnode);


  //discforhat
  disc->set(0.65*s, 0.0, 2*M_PI, 36)->setColor(0, 1, 0, 1)->load();
  tnode = new cs475::HNode(model1.nodes[26],disc->getVertexCount());
  tnode->change_parameters(0,0,0.75*s,0,0,0);
  (model1.nodes).push_back(tnode);



  model1.curr_node = model1.nodes[0];
  model1.limb = 0;


  //--------------------------- Model 2 (MUSIC BOX) ---------------------------------//

  cuboid->set(0.0*s2, 0.0*s2, -0.0*s2, 0*s2, -0.0*s2, 0.0*s2)->setColor(1.0,1.0,0,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(0,0,0,0,0,0);
  (model2.nodes).push_back(tnode);

  cuboid->set(-6.0*s2, 6.0*s2, -5.0*s2, 5*s2, -0.1*s2, 0.1*s2)->setColor(1.0,1.0,0,1)->load(0);
  tnode = new cs475::HNode(model2.nodes[0],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(0,0,-6*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(0,0,12.0*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(-0.1*s2, 0.1*s2, -5.0*s2, 5*s2, -5.9*s2, 5.9*s2)->setColor(1,0,1,1)->load(0);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(-5.9*s2,0,6.0*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(5.9*s2,0,6.0*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(-6*s2, 6*s2, -0.1*s2, 0.1*s2, -6.1*s2, 6.1*s2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(0,-5.1*s2,6.0*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  cuboid->set(12*s2, 0, 0.2*s2, 0, 12.2*s2, 0)->setColor(0,1,1,1)->load(1);
  tnode = new cs475::HNode(model2.nodes[1],cuboid->getVertexCount(), wooden, true);
  tnode->change_parameters(-6*s2,5*s2,-0.1*s2,0,0,0);
  (model2.nodes).push_back(tnode);
  

  model2.curr_node = model2.nodes[0];
  model2.limb = 0;

  

  //-------------------------- Model 3 (R2D2) -------------------------------//



  // main-body - 0
  int m3_0_r=10, m3_0_half_h=15;
  double m3_rot_body=20;
  cone->set(m3_0_r*s3, m3_0_r*s3, -m3_0_half_h*s3, m3_0_half_h*s3, 0, 2*M_PI, 360)->load();
  tnode = new cs475::HNode(NULL,cone->getVertexCount());
  tnode->change_parameters(m2_l/4,0,0,-(90+m3_rot_body),15.0,-30.0);
  (model3.nodes).push_back(tnode);

  // head - 1
  sphere->set(m3_0_r*s3, m3_0_r*s3, m3_0_r*s3, 0, M_PI, 0, 2*M_PI, 360, true)->setColor(0.26,0.52,0.89,1)->load();
  tnode = new cs475::HNode(model3.nodes[0],sphere->getVertexCount());
  tnode->change_parameters(0.0,0.0,m3_0_half_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // botton-main-rod - 2
  int m3_2_half_w=2, m3_2_h=4;
  cuboid->set(-m3_2_half_w*s3, m3_2_half_w*s3, -m3_2_half_w*s3, m3_2_half_w*s3, m3_2_h*s3, -m3_2_h*s3)->setColor(0.26,0.52,0.89,1)->load(0);
  tnode = new cs475::HNode(model3.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(0.0,0.0,-m3_0_half_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // botton-main-feet - 3
  int m3_3_half_w1=2, m3_3_half_w2=7, m3_3_h=8;
  double m3_rot=50;
  trapezoid->set(m3_3_half_w1*s3, m3_3_half_w2*s3, m3_3_half_w1*s3, m3_3_half_w2*s3, 0, -m3_3_h*s3)->load(1);
  tnode = new cs475::HNode(model3.nodes[2],trapezoid->getVertexCount());
  tnode->change_parameters(0.0,0.0,-m3_2_h*s3,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand Elbow - 4
  int m3_4_r=5, m3_4_half_h=3;
  cone->set(m3_4_r*s3, m3_4_r*s3, -m3_4_half_h*s3, m3_4_half_h*s3, 0, M_PI, 360)->setColor(0.26,0.52,0.89,1)->load();
  tnode = new cs475::HNode(model3.nodes[0],cone->getVertexCount());
  tnode->change_parameters(m3_0_r*s3,0,(m3_0_half_h-m3_4_r)*s3,90,90,m3_rot);
  (model3.nodes).push_back(tnode);

  // 5
  tnode = new cs475::HNode(model3.nodes[0],cone->getVertexCount());
  tnode->change_parameters(-m3_0_r*s3,0,(m3_0_half_h-m3_4_r)*s3,90,90,m3_rot);
  (model3.nodes).push_back(tnode);

  // Hand Arm - 6
  int m3_5_half_h=16;
  cuboid->set(-m3_4_r*s3, m3_4_r*s3, 0, 2*m3_5_half_h*s3, 0, m3_4_half_h*s3)->setColor(0.26,0.52,0.89,1)->load(0);
  tnode = new cs475::HNode(model3.nodes[4],cuboid->getVertexCount());
  tnode->change_parameters(0.0,-2*m3_5_half_h*s3,0,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // 7
  cuboid->set(-m3_4_r*s3, m3_4_r*s3, 0, 2*m3_5_half_h*s3, -m3_4_half_h*s3, 0)->setColor(0.26,0.52,0.89,1)->load(0);
  tnode = new cs475::HNode(model3.nodes[5],cuboid->getVertexCount());
  tnode->change_parameters(0.0,-2*m3_5_half_h*s3,0,0,0.0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand End - 8, 9

  trapezoid->set(m3_4_r*s3, 1.7*m3_4_r*s3, (m3_4_half_h*s3)/2, (2.5*m3_4_half_h*s3)/2, 0, 5*s3)->load(1);
  tnode = new cs475::HNode(model3.nodes[6],trapezoid->getVertexCount());
  tnode->change_parameters(0.0,0.0,(m3_4_half_h*s3)/2,90,0,0.0);
  (model3.nodes).push_back(tnode);

  trapezoid->set(m3_4_r*s3, 1.7*m3_4_r*s3, (m3_4_half_h*s3)/2, (2.5*m3_4_half_h*s3)/2, 0, 5*s3)->load(1);
  tnode = new cs475::HNode(model3.nodes[7],trapezoid->getVertexCount());
  tnode->change_parameters(0.0,0.0,-(m3_4_half_h*s3)/2,90,0,0.0);
  (model3.nodes).push_back(tnode);

  // Hand Discs - 10, 11

  disc->set(m3_4_r*s3,0,M_PI,360)->setColor(0.26,0.52,0.89,1)->load();
  tnode = new cs475::HNode(model3.nodes[4],disc->getVertexCount());
  tnode->change_parameters(0.0,0.0,m3_4_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  tnode = new cs475::HNode(model3.nodes[5],disc->getVertexCount());
  tnode->change_parameters(0.0,0.0,-m3_4_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  // Base Disc - 12

  disc->set(m3_0_r*s3,0,2*M_PI,360)->load();
  tnode = new cs475::HNode(model3.nodes[0],disc->getVertexCount());
  tnode->change_parameters(0.0,0.0,-m3_0_half_h*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);

  // Eye - 13

  cone->set(2*s3, 2*s3, -m3_4_half_h*s3, (m3_4_half_h*0.75)*s3, 0, 2*M_PI, 360)->load();
  tnode = new cs475::HNode(model3.nodes[1],cone->getVertexCount());
  tnode->change_parameters(0,-10*s3,2.5*s3,80,0,0);
  (model3.nodes).push_back(tnode);

  // 14
  disc->set(2*s3,0,2*M_PI,360)->load();
  tnode = new cs475::HNode(model3.nodes[13],disc->getVertexCount());
  tnode->change_parameters(0.0,0.0,m3_4_half_h*0.75*s3,0,0,0.0);
  (model3.nodes).push_back(tnode);  


  model3.curr_node = model3.nodes[0];
  model3.limb = 0;
  

  //-------------------------- Model 4 (MAIN ROOM) -------------------------------//
  

  cuboid->set(0.0*s4, 0.0*s4, -0.0*s4, 0*s4, -0.0*s4, 0.0*s4)->setColor(1.0,1.0,0,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(0,0,0,0,0,0);
  (model4.nodes).push_back(tnode);
  

  cuboid->set(-5.2*s4, 5.2*s4, -4.2*s4, 4.2*s4, -0.1*s4, 0.1*s4)->setColor(1.0,1.0,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(0,0,-5.1*s4,0,0,0);
  (model4.nodes).push_back(tnode);

  cuboid->set(-5.2*s4, 5.2*s4, -0.1*s4, 0.1*s4, -5.0*s4, 5.0*s4)->setColor(1.0,0.0,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),carpet, true);
  tnode->change_parameters(0,-4.1*s4,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-5.2*s4, 5.2*s4, -0.1*s4, 0.1*s4, -5.0*s4, 5.0*s4)->setColor(1.0,0.0,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),roof, true);
  tnode->change_parameters(0,4.1*s4,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-0.1*s4, 0.1*s4,-4.0*s4, 4.0*s4,-5.0*s4, 5.0*s4)->setColor(0.0,0.0,1,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(-5.1*s4,0,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-5.2*s4, 5.2*s4, -1.6*s4, 1.6*s4, -0.1*s4, 0.1*s4)->setColor(0.5,0.5,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(0,2.6*s4,5.1*s4,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-1.6*s4, 1.6*s4, -2.6*s4, 2.6*s4, -0.1*s4, 0.1*s4)->setColor(0.5,0.5,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(3.6*s4,-1.6*s4,5.1*s4,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-1.6*s4, 1.6*s4, -2.6*s4, 2.6*s4, -0.1*s4, 0.1*s4)->setColor(0.5,0.5,0,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(-3.6*s4,-1.6*s4,5.1*s4,0,0,0);
  (model4.nodes).push_back(tnode);

  cuboid->set(4*s4, 0, 5.0*s4, 0, 0.2*s4, 0)->setColor(0.0,1,0,1)->load(1);
  tnode = new cs475::HNode(model4.nodes[6],cuboid->getVertexCount(),metal, true);
  tnode->change_parameters(-1.6*s4,-2.4*s4,0.1*s4,0,180,0);
  (model4.nodes).push_back(tnode);

  cuboid->set(-0.1*s4, 0.1*s4,1.0*s4, 4.0*s4,-5.0*s4, 5.0*s4)->setColor(0.0,0.0,1,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(5.1*s4,0,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-0.1*s4, 0.1*s4,-4.0*s4, -1.0*s4,-5.0*s4, 5.0*s4)->setColor(0.0,0.0,1,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(5.1*s4,0,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(-0.1*s4, 0.1*s4,-1.0*s4, 1.0*s4,-5.0*s4, -1.0*s4)->setColor(0.0,0.0,1,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(5.1*s4,0,0,0,0,0);
  (model4.nodes).push_back(tnode);
  
  cuboid->set(-0.1*s4, 0.1*s4,-1.0*s4, 1.0*s4,1.0*s4, 5.0*s4)->setColor(0.0,0.0,1,1)->load(0);
  tnode = new cs475::HNode(model4.nodes[0],cuboid->getVertexCount(),wood, true);
  tnode->change_parameters(5.1*s4,0,0,0,0,0);
  (model4.nodes).push_back(tnode);


  cuboid->set(0.2*s4, 0,2.0*s4,0,2.0*s4, 0)->setColor(1.0,1.0,1,1)->load(1);
  tnode = new cs475::HNode(model4.nodes[12],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-0.1*s4,-1.0*s4,-1.0*s4,0,0,0);
  (model4.nodes).push_back(tnode);

  model4.curr_node = model4.nodes[8];
  model4.limb = 0;

  //-------------------------- Model 5 (TABLE) -------------------------------//

  double m5_0_l = 18*s5, m5_0_b = 15*s5, m5_0_h = 4*s5; // table top
  double m5_1_l = 1*s5, m5_1_b = 1*s5, m5_1_h = (m4_h/2-m5_0_h-m2_h/2)*s5; // table legs

  // table top = 0
  cuboid->set(-m5_0_l/2, m5_0_l/2, -m5_0_h/2, m5_0_h/2, -m5_0_b/2, m5_0_b/2)->setColor(1,0,0,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(), marble, true);
  tnode->change_parameters(0,-m4_h/2+m5_0_h/2+m5_1_h,0,0,0,0);
  (model5.nodes).push_back(tnode);

  // table legs = 1,2,3,4
  cuboid->set(-m5_1_l/2, m5_1_l/2, -m5_1_h/2, m5_1_h/2, -m5_1_b/2, m5_1_b/2)->setColor(0,1,0,1)->load(0);
  tnode = new cs475::HNode(model5.nodes[0],cuboid->getVertexCount(), wood, true);
  tnode->change_parameters((m5_0_l-m5_1_l)/2,-(m5_0_h+m5_1_h)/2,(m5_0_b-m5_1_b)/2,0,0,0);
  (model5.nodes).push_back(tnode);

  tnode = new cs475::HNode(model5.nodes[0],cuboid->getVertexCount(), wood, true);
  tnode->change_parameters((m5_0_l-m5_1_l)/2,-(m5_0_h+m5_1_h)/2,-(m5_0_b-m5_1_b)/2,0,0,0);
  (model5.nodes).push_back(tnode);

  tnode = new cs475::HNode(model5.nodes[0],cuboid->getVertexCount(), wood, true);
  tnode->change_parameters(-(m5_0_l-m5_1_l)/2,-(m5_0_h+m5_1_h)/2,-(m5_0_b-m5_1_b)/2,0,0,0);
  (model5.nodes).push_back(tnode);

  tnode = new cs475::HNode(model5.nodes[0],cuboid->getVertexCount(), wood, true);
  tnode->change_parameters(-(m5_0_l-m5_1_l)/2,-(m5_0_h+m5_1_h)/2,(m5_0_b-m5_1_b)/2,0,0,0);
  (model5.nodes).push_back(tnode);

  model5.curr_node = model5.nodes[0];
  model5.limb = 0;

  //-------------------------- Model 6 (CHAIR) -------------------------------//

  double m6_0_l = 8*s6, m6_0_b = 8*s6, m6_0_h = 1*s6, m6_0_toff_x = 15*s6, m6_0_toff_z = 0*s6; // chair top
  double m6_1_l = 0.8*s6, m6_1_b = 0.8*s6, m6_1_h = 8*s6; // chair legs

  // chair top = 0
  cuboid->set(-m6_0_l/2, m6_0_l/2, -m6_0_h/2, m6_0_h/2, -m6_0_b/2, m6_0_b/2)->setColor(1,1,1,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount());
  tnode->change_parameters(m6_0_toff_x,-m4_h/2+m6_0_h/2+m6_1_h,m6_0_toff_z,0,0,0);
  (model6.nodes).push_back(tnode);

  // chair legs = 1,2,3,4
  cuboid->set(-m6_1_l/2, m6_1_l/2, -m6_1_h/2, m6_1_h/2, -m6_1_b/2, m6_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model6.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters((m6_0_l-m6_1_l)/2,-(m6_0_h+m6_1_h)/2,(m6_0_b-m6_1_b)/2,0,0,0);
  (model6.nodes).push_back(tnode);

  tnode = new cs475::HNode(model6.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters((m6_0_l-m6_1_l)/2,-(m6_0_h+m6_1_h)/2,-(m6_0_b-m6_1_b)/2,0,0,0);
  (model6.nodes).push_back(tnode);

  tnode = new cs475::HNode(model6.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(-(m6_0_l-m6_1_l)/2,-(m6_0_h+m6_1_h)/2,-(m6_0_b-m6_1_b)/2,0,0,0);
  (model6.nodes).push_back(tnode);

  tnode = new cs475::HNode(model6.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(-(m6_0_l-m6_1_l)/2,-(m6_0_h+m6_1_h)/2,(m6_0_b-m6_1_b)/2,0,0,0);
  (model6.nodes).push_back(tnode);

  model6.curr_node = model6.nodes[0];
  model6.limb = 0;

  //-------------------------- Model 7 (LAMP) -------------------------------//


  double m7_1_l = 3.5*s7, m7_1_b = 3.5*s7, m7_1_h = 1.8*s7; // lamp base
  double m7_0_l = 0.8*s7, m7_0_b = 0.8*s7, m7_0_h = 25*s7; // lamp leg
  double m7_2_l = 7*s7, m7_2_b = 7*s7, m7_2_h = 4*s7; // lamp cover

  // lamp leg = 0
  cuboid->set(-m7_0_l/2, m7_0_l/2, -m7_0_h/2, m7_0_h/2, -m7_0_b/2, m7_0_b/2)->setColor(1,0,1,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-m4_l/2+m7_2_l,-m4_h/2+m7_1_h+m7_0_h/2,-m4_b/2+m7_2_b,0,0,0);
  (model7.nodes).push_back(tnode);

  // lamp base = 1
  trapezoid->set(m7_0_l/2, m7_1_l/2, m7_0_b/2, m7_1_b/2, -m7_1_h/2, m7_1_h/2)->setColor(0,1,1,1)->load(1);
  tnode = new cs475::HNode(model7.nodes[0],trapezoid->getVertexCount());
  tnode->change_parameters(0,-(m7_0_h+m7_1_h)/2,0,90,0,0);
  (model7.nodes).push_back(tnode);

  // lamp cover = 2
  trapezoid->set(m7_0_l/2, m7_2_l/2, m7_0_b/2, m7_2_b/2, -m7_2_h/2, m7_2_h/2)->setColor(1,1,1,1)->load(1);
  tnode = new cs475::HNode(model7.nodes[0],trapezoid->getVertexCount());
  tnode->change_parameters(0,(m7_0_h+m7_2_h)/2,0,90,0,0);
  (model7.nodes).push_back(tnode);

  //-------------------------- Model 8 (CHAIR 2) -------------------------------//

  double m8_0_l = 8*s8, m8_0_b = 8*s8, m8_0_h = 1*s8, m8_0_toff_x = 15*s8, m8_0_toff_z = 0*s8; // chair top
  double m8_1_l = 0.8*s8, m8_1_b = 0.8*s8, m8_1_h = 8*s8; // chair legs

  // chair top = 0
  cuboid->set(-m8_0_l/2, m8_0_l/2, -m8_0_h/2, m8_0_h/2, -m8_0_b/2, m8_0_b/2)->setColor(1,1,1,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount());
  tnode->change_parameters(-m8_0_toff_x,-m4_h/2+m8_0_h/2+m8_1_h,m8_0_toff_z,0,0,0);
  (model8.nodes).push_back(tnode);

  // chair legs = 1,2,3,4
  cuboid->set(-m8_1_l/2, m8_1_l/2, -m8_1_h/2, m8_1_h/2, -m8_1_b/2, m8_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model8.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters((m8_0_l-m8_1_l)/2,-(m8_0_h+m8_1_h)/2,(m8_0_b-m8_1_b)/2,0,0,0);
  (model8.nodes).push_back(tnode);

  tnode = new cs475::HNode(model8.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters((m8_0_l-m8_1_l)/2,-(m8_0_h+m8_1_h)/2,-(m8_0_b-m8_1_b)/2,0,0,0);
  (model8.nodes).push_back(tnode);

  tnode = new cs475::HNode(model8.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(-(m8_0_l-m8_1_l)/2,-(m8_0_h+m8_1_h)/2,-(m8_0_b-m8_1_b)/2,0,0,0);
  (model8.nodes).push_back(tnode);

  tnode = new cs475::HNode(model8.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(-(m8_0_l-m8_1_l)/2,-(m8_0_h+m8_1_h)/2,(m8_0_b-m8_1_b)/2,0,0,0);
  (model8.nodes).push_back(tnode);

  model8.curr_node = model8.nodes[0];
  model8.limb = 0;

  //-------------------------- Model 9 (CORNER RACK) -------------------------------//

  double m9_0_l = 8*s9, m9_0_b = 8*s9, m9_0_h = 1*s9, m9_0_toff_x = (m4_l-m9_0_l)/2, m9_0_toff_z = -(m4_b-m9_0_b)/2; // rack top
  double m9_1_l = 0.8*s9, m9_1_b = 0.8*s9, m9_1_h = 8*s9; // rack legs

  // rack top = 0
  cuboid->set(-m9_0_l/2, m9_0_l/2, -m9_0_h/2, m9_0_h/2, -m9_0_b/2, m9_0_b/2)->setColor(0,0,1,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount());
  tnode->change_parameters(m9_0_toff_x,-m4_h/2+m9_0_h/2+m9_1_h,m9_0_toff_z,0,0,0);
  (model9.nodes).push_back(tnode);

  // rack legs = 1,2,3,4
  cuboid->set(-m9_1_l/2, m9_1_l/2, -m9_1_h/2, m9_1_h/2, -m9_1_b/2, m9_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model9.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  // rack_2 top = 5
  cuboid->set(-m9_0_l/2, m9_0_l/2, -m9_0_h/2, m9_0_h/2, -m9_0_b/2, m9_0_b/2)->setColor(0,1,0,1)->load(0);
  tnode = new cs475::HNode(model9.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(0,m9_0_h+m9_1_h,0,0,0,0);
  (model9.nodes).push_back(tnode);

  // rack_2 legs = 6,7,8,9
  cuboid->set(-m9_1_l/2, m9_1_l/2, -m9_1_h/2, m9_1_h/2, -m9_1_b/2, m9_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model9.nodes[5],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[5],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[5],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[5],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  // rack_3 top = 10
  cuboid->set(-m9_0_l/2, m9_0_l/2, -m9_0_h/2, m9_0_h/2, -m9_0_b/2, m9_0_b/2)->setColor(1,0,0,1)->load(0);
  tnode = new cs475::HNode(model9.nodes[5],cuboid->getVertexCount());
  tnode->change_parameters(0,m9_0_h+m9_1_h,0,0,0,0);
  (model9.nodes).push_back(tnode);

  // rack_3 legs = 11,12,13,14
  cuboid->set(-m9_1_l/2, m9_1_l/2, -m9_1_h/2, m9_1_h/2, -m9_1_b/2, m9_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model9.nodes[10],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[10],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[10],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,-(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  tnode = new cs475::HNode(model9.nodes[10],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m9_0_l-m9_1_l)/2,-(m9_0_h+m9_1_h)/2,(m9_0_b-m9_1_b)/2,0,0,0);
  (model9.nodes).push_back(tnode);

  model9.curr_node = model9.nodes[0];
  model9.limb = 0;

  //-------------------------- Model 10 (WALL LIGHT) -------------------------------//

  double m10_0_rs = 1*s10, m10_0_rb = 2*s10, m10_0_h = 4*s10, m10_0_goff = 0.75*m4_h;
  
  cone->set(m10_0_rs, m10_0_rb, 0, m10_0_h, 0, M_PI, 360)->load();
  tnode = new cs475::HNode(NULL,cone->getVertexCount());
  tnode->change_parameters(-m4_l/2,-m4_h/2+m10_0_goff,0,-90,0,-90);
  (model10.nodes).push_back(tnode);
  
  //-------------------------- Model 11 (ROYAL CHAIR) -------------------------------//

  double m11_0_l = 9*s11, m11_0_b = 9*s11, m11_0_h = 1*s11, m11_0_toff_x = -(m4_l-m11_0_l*1.5)/2, m11_0_toff_z = (m4_b-m11_0_b*1.5)/2; // chair top
  double m11_1_l = 0.9*s11, m11_1_b = 0.9*s11, m11_1_h = 10*s11; // chair legs
  double m11_2_h = 9*s11;

  // chair top = 0
  cuboid->set(-m11_0_l/2, m11_0_l/2, -m11_0_h/2, m11_0_h/2, -m11_0_b/2, m11_0_b/2)->setColor(1,0.2,0.3,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount());
  tnode->change_parameters(m11_0_toff_x,-m4_h/2+m11_0_h/2+m11_1_h,m11_0_toff_z,0,0,0);
  (model11.nodes).push_back(tnode);

  // chair legs = 1,2,3,4
  cuboid->set(-m11_1_l/2, m11_1_l/2, -m11_1_h/2, m11_1_h/2, -m11_1_b/2, m11_1_b/2)->setColor(0,1,1,1)->load(0);
  tnode = new cs475::HNode(model11.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m11_0_l-m11_1_l)/2,-(m11_0_h+m11_1_h)/2,(m11_0_b-m11_1_b)/2,0,0,0);
  (model11.nodes).push_back(tnode);

  tnode = new cs475::HNode(model11.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters((m11_0_l-m11_1_l)/2,-(m11_0_h+m11_1_h)/2,-(m11_0_b-m11_1_b)/2,0,0,0);
  (model11.nodes).push_back(tnode);

  tnode = new cs475::HNode(model11.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m11_0_l-m11_1_l)/2,-(m11_0_h+m11_1_h)/2,-(m11_0_b-m11_1_b)/2,0,0,0);
  (model11.nodes).push_back(tnode);

  tnode = new cs475::HNode(model11.nodes[0],cuboid->getVertexCount(), metal, true);
  tnode->change_parameters(-(m11_0_l-m11_1_l)/2,-(m11_0_h+m11_1_h)/2,(m11_0_b-m11_1_b)/2,0,0,0);
  (model11.nodes).push_back(tnode);

  // chair backrest
  cuboid->set(-m11_1_l/2, m11_1_l/2, -m11_2_h/2, m11_2_h/2, -m11_0_b/2, m11_0_b/2)->setColor(0.3,0.3,0.8,1)->load(0);
  tnode = new cs475::HNode(model11.nodes[0],cuboid->getVertexCount());
  tnode->change_parameters(-(m11_0_l-m11_1_l)/2,(m11_0_h+m11_2_h)/2,0,0,0,0);
  (model11.nodes).push_back(tnode);

  model11.curr_node = model11.nodes[0];
  model11.limb = 0;






  cuboid->set(0.0*s4, 0.0*s4, -0.0*s4, 0*s4, -0.0*s4, 0.0*s4)->setColor(1.0,1.0,0,1)->load(0);
  tnode = new cs475::HNode(NULL,cuboid->getVertexCount(),wooden, true);
  tnode->change_parameters(0,0,0,0,0,0);
  (model12.nodes).push_back(tnode);









}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform1f(uLight1,light1);
  glUniform1f(uLight2,light2);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix

  if(mode == 1)
  {
    glm::vec4 ccpos = glm::vec4(c_xpos,c_ypos,0, 1.0)*c_rotation_matrix;
    lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(ccpos),glm::vec3(c_up));
  }
  else
  {
    lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));  
  }
  

  //creating the projection matrix
  if(enable_perspective)
    //projection_matrix = glm::frustum(-10.0, 10.0, -10.0, 10.0, 5.0, 40.0);
    projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.001,1000.0);
  else
    projection_matrix = glm::ortho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);

  view_matrix = projection_matrix*lookat_matrix;

  glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

  matrixStack.push_back(view_matrix);

  model1.nodes[0]->render_tree();
  model2.nodes[0]->render_tree();
  model3.nodes[0]->render_tree();
  model4.nodes[0]->render_tree();
  model5.nodes[0]->render_tree();
  model6.nodes[0]->render_tree();
  model7.nodes[0]->render_tree();
  model8.nodes[0]->render_tree();
  model9.nodes[0]->render_tree();
  model10.nodes[0]->render_tree();
  model11.nodes[0]->render_tree();

  if(mode == 1)
  {
    model12.nodes[0]->render_tree();
  }
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
  window = glfwCreateWindow(screen_width, screen_height, "Assignment-3 Animation", NULL, NULL);
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

  //Mouse Callback
  glfwSetMouseButtonCallback(window, cs475::mouse_button_callback);

  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, cs475::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  cs475::initGL();
  initBuffersGL();

  camera_points.push_back(glm::vec3(c_xpos,6.0,20.0));
  float time1 = 0.1f;
  float time2 = 0.04f;

  float previous = -1;

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      if(mode == 2)
      {
        if(previous < 0) previous = glfwGetTime();
        float now = glfwGetTime();
        //std::cout<<now<<std::endl;
        float delta = now - previous;
        previous = now;

        //std::cout<<camera_points.size()<<std::endl; 

        time1 -= delta;
        if (time1 <= 0.0)
        {
            c_xpos = camera_movement[camera_movement.size()-1-campoint][0];
            c_ypos = camera_movement[camera_movement.size()-1-campoint][1];
            c_zpos = camera_movement[camera_movement.size()-1-campoint][2];
            time1 = 0.1;
            campoint++;
            

            if(campoint == camera_movement.size())
            {
              mode = 3;
              previous = -1;
              readKeyframes();
            }
        }
  
      }
      if(mode == 3)
      {
        if(previous < 0) previous = glfwGetTime();
        float now = glfwGetTime();
        float delta = now - previous;
        previous = now;


        time2 -= delta;
        if (time2 <= 0.0)
        {
            applyFrame(framenum);
            time2 = 0.04;
            framenum++;
            
            // Enable if you want to store the frames as images
            // captureFrame();

            if(framenum == allframes.size())
            {
              mode = 4;
              previous = -1;
            }
        }
      }
      
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

