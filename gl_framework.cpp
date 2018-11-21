#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#include "defs.hpp"

extern GLfloat c_xrot,c_yrot,c_zrot,c_zpos,c_xpos,c_ypos;
extern bool enable_perspective;
extern cs475::Model model1, model2, model3, model4;
extern glm::mat4 view_matrix, modelview_matrix;
extern GLfloat c_zpos;
extern std::vector<glm::vec3> camera_points;
extern std::vector<glm::vec3> camera_movement;
extern int mode, screen_height, screen_width;

extern float light1,light2;

extern void dumpFrame();
extern void readKeyframes();
extern void drawSphere(glm::vec3 center);
extern cs475::Model model4;

int factorial(int n)
{
  if(n==0) return 1;

  int ret = 1;
    for(int i = 1;i<=n;i++)
    {
      ret *= i;
    }

    return ret;
}

float binomial_coff(float n,float k)
{
    float ans;
    ans = factorial(n) / (factorial(k)*factorial(n-k));
    return ans;
}


glm::vec3 drawBezierPoint(std::vector<glm::vec3> &points, double t)
{
  glm::vec3 P = glm::vec3(0.0);
    
    size_t num_points = points.size();
    for (int i = 0; i<num_points; i++)
    {
        P.x = P.x + binomial_coff((float)(num_points - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (num_points - 1 - i)) * points[i].x;
        P.y = P.y + binomial_coff((float)(num_points - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (num_points - 1 - i)) * points[i].y;
        P.z = P.z + binomial_coff((float)(num_points - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (num_points - 1 - i)) * points[i].z;
    }
    
    return P;
}


void drawBezier(std::vector<glm::vec3> &points, double delta_t)
{
  for(double t = 0.0;t <= 1.0; t += delta_t)
    {
        glm::vec3 p = drawBezierPoint(points,t);
        camera_movement.push_back(p);
    }
}


namespace cs475
{

  int model = 4;

  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewPort to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  

  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if(button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
    {
        GLfloat x = (xpos-screen_width/2)/screen_width;
        GLfloat y = (screen_height/2-ypos)/screen_height;
        glm::vec4 pos = glm::vec4(x,y,c_zpos,1);
        glm::mat4 inv_view = inverse(view_matrix);

        glm::vec4 newpos = pos*inv_view;
        newpos.z = c_zpos;
        newpos.y = newpos.y + c_ypos;
        newpos.x = newpos.x + c_xpos;

        drawSphere(glm::vec3(newpos));
        camera_points.push_back(glm::vec3(newpos));
    }
  }


  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);

    else if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
      dumpFrame();
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
      remove("keyframes.txt");
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
      light1 = -light1;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
      light2 = -light2;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
      readKeyframes(); 
    }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
      mode = 2;
      
      drawBezier(camera_points,0.02);
    }
    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
      std::cout<<"\n\n";
      std::cout<<"Model1 :  0\n";
      std::cout<<"Model2 :  1\n";
      std::cout<<"Model3 :  2\n";
      std::cout<<"Model4 :  3\n";
      std::cout<<"Enter the Corresponding Number to Select: ";
      int d; std::cin>>d;

      model = d+1;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {

      if(model == 1)
      {
        std::cout<<"\n\n";

        std::cout<<"Head :  0\n";
        std::cout<<"Torso:  1\n";
        std::cout<<"RArmU:  2\n";
        std::cout<<"LArmU:  3\n";
        std::cout<<"RLegU:  4\n";
        std::cout<<"LLegU:  5\n";
        std::cout<<"RArmL:  6\n";
        std::cout<<"LArmL:  7\n";
        std::cout<<"RlegL:  8\n";
        std::cout<<"LlegL:  9\n";
        std::cout<<"Enter the Corresponding Number to Select: ";
        

        int d=0; std::cin>>d;
        switch(d)
        {
          case 0: model1.limb = 2; break;
          case 1: model1.limb = 0; break;
          case 2: model1.limb = 4; break;
          case 3: model1.limb = 5; break;
          case 4: model1.limb = 13; break;
          case 5: model1.limb = 14; break;
          case 6: model1.limb = 8; break;
          case 7: model1.limb = 9; break;
          case 8: model1.limb = 17; break;
          case 9: model1.limb = 18; break;
          default: break;
        }

        model1.curr_node = model1.nodes[model1.limb];
      }

      if(model == 2)
      {
        std::cout<<"\n\n";

        std::cout<<"Lid :  0\n";
        std::cout<<"Box:   1\n";
        std::cout<<"Enter the Corresponding Number to Select: ";
        int d=0; std::cin>>d;

        switch(d)
        {
          case 0: model2.limb = 6; break;
          case 1: model2.limb = 0; break;
          default: break;
        }
        model2.curr_node = model2.nodes[model2.limb];

      }

      if(model == 3)
      {
        std::cout<<"\n\n";

        std::cout<<"Head :  0\n";
        std::cout<<"Torso:  1\n";
        std::cout<<"RArm:   2\n";
        std::cout<<"LArm:   3\n";
        std::cout<<"Leg:    4\n";

        std::cout<<"Enter the Corresponding Number to Select: ";
        int d=0; std::cin>>d;
        switch(d)
        {
          case 0: model3.limb = 1; break;
          case 1: model3.limb = 0; break;
          case 2: model3.limb = 4; break;
          case 3: model3.limb = 5; break;
          case 4: model3.limb = 2; break;
          default: break;
        }
        model3.curr_node = model3.nodes[model3.limb];
      }

      if(model == 4)
      {
        std::cout<<"\n\n";

        std::cout<<"Door :  0\n";
        std::cout<<"Window:  1\n";
        
        std::cout<<"Enter the Corresponding Number to Select: ";
        int d=0; std::cin>>d;
        switch(d)
        {
          case 0: model4.curr_node = model4.nodes[8]; break;
          case 1: model4.curr_node = model4.nodes[13]; break;
          default: break;
        }
        
      }
      
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(model1.limb != 8 && model1.limb != 9 && model1.limb != 17 && model1.limb != 18)
        {
          model1.curr_node->dec_ry();   
        }
      }
        
       if(model == 2)
      {
        if(model2.limb != 6)
        {
          model2.curr_node->dec_ry();
        }
      }

      if(model == 3)
      {
        if(model3.limb != 4 && model3.limb != 5 && model3.limb != 1)
        {
          model3.curr_node->dec_ry();   
        }
      }

      if(model == 4)
      {
        model4.curr_node->dec_ry();
      }

    }
    else if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(model1.limb != 8 && model1.limb != 9 && model1.limb != 17 && model1.limb != 18)
        {
          model1.curr_node->inc_ry();   
        }
      } 

       if(model == 2)
      {
        if(model2.limb != 6)
        {
          model2.curr_node->inc_ry();
        }
      }

      if(model == 3)
      {
        if(model3.limb != 4 && model3.limb != 5 && model3.limb != 1)
        {
          model3.curr_node->inc_ry();   
        }
      }

      if(model == 4)
      {
        model4.curr_node->inc_ry();
      }

    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(true)
        {
          model1.curr_node->dec_rx();   
        } 
      }
      if(model == 2)
      {
        model2.curr_node->dec_rx();   
      }
      if(model == 3)
      {
        if(model3.limb != 1)
        {
          model3.curr_node->dec_rx();
        }
      }
      
    }
    else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(true)
        {
          model1.curr_node->inc_rx();   
        }
      }
      if(model == 2)
      {
        model2.curr_node->inc_rx();
      }
      if(model == 3)
      {
        if(model3.limb != 1)
        {
          model3.curr_node->inc_rx();
        }
         
      }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(model1.limb != 8 && model1.limb != 9 && model1.limb != 17 && model1.limb != 18)
        {
          model1.curr_node->dec_rz();   
        }  
      }

       if(model == 2)
      {
        if(model2.limb != 6)
        {
          model2.curr_node->dec_rz();
        }
      }

      if(model == 3)
      {
        if(model3.limb != 4 && model3.limb != 5)
        {
          model3.curr_node->dec_rz();   
        }
        
      }
      
    }
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
      if(model == 1)
      {
        if(model1.limb != 8 && model1.limb != 9 && model1.limb != 17 && model1.limb != 18)
        {
          model1.curr_node->inc_rz();   
        } 
      }
      
      if(model == 2)
      {
        if(model2.limb != 6)
        {
          model2.curr_node->inc_rz();
        }
      }

      if(model == 3)
      {
        if(model3.limb != 4 && model3.limb != 5)
        {
          model3.curr_node->inc_rz();   
        }
      }
       

    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
      enable_perspective = !enable_perspective;   
    }
    else if (key == GLFW_KEY_A  && action == GLFW_PRESS)
      c_yrot -= 15.0;
    else if (key == GLFW_KEY_D  && action == GLFW_PRESS)
      c_yrot += 15.0;
    else if (key == GLFW_KEY_W  && action == GLFW_PRESS)
      c_xrot -= 15.0;
    else if (key == GLFW_KEY_S  && action == GLFW_PRESS)
      c_xrot += 15.0;        
    else if (key == GLFW_KEY_Q  && action == GLFW_PRESS)
      c_zrot -= 15.0;
    else if (key == GLFW_KEY_E  && action == GLFW_PRESS)
      c_zrot += 15.0;


    else if (key == GLFW_KEY_J  && action == GLFW_PRESS)
      c_xpos -= 1.0;
    else if (key == GLFW_KEY_L  && action == GLFW_PRESS)
      c_xpos += 1.0;
    else if (key == GLFW_KEY_I  && action == GLFW_PRESS)
      c_zpos -= 1.0;
    else if (key == GLFW_KEY_K  && action == GLFW_PRESS)
      c_zpos += 1.0;        
    else if (key == GLFW_KEY_U  && action == GLFW_PRESS)
      c_ypos -= 1.0;
    else if (key == GLFW_KEY_O  && action == GLFW_PRESS)
      c_ypos += 1.0;   
  }
};  
  


