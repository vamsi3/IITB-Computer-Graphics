#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"
#include "defs.hpp"

namespace cs475	 { 

	// A class that represents a node in the hierarchy tree
	class HNode {
		GLfloat tx,ty,tz,rx,ry,rz;

		bool use_texture = false;

		GLuint num_vertices;
		GLuint vao,vbo,tex;

		glm::mat4 rotation;
		glm::mat4 translation;
		
		std::vector<HNode*> children;
		HNode* parent;

		void update_matrices();

	  public:
		HNode (HNode*, GLuint, GLuint, bool use_texture = false);
		
		void add_child(HNode*);
		void render();
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void render_tree();
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );


	class Model {

	public:
		std::vector<cs475::HNode*> nodes;
		cs475::HNode* curr_node;
		int limb;
	};

};	

#endif