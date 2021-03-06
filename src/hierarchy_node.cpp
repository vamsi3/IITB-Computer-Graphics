#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition,vColor,vNormal,vTexcord,vUseTexture,uModelViewMatrix,uViewMatrix,uNormalMatrix;
extern std::vector<glm::mat4> matrixStack;

namespace cs475
{

	HNode::HNode(HNode* a_parent, GLuint num_v, GLuint texture, bool use_texture){

		num_vertices = num_v;
		tex = texture;
		std::size_t data_size = sizeof(glm::vec2)*num_vertices/2;

		this->use_texture = use_texture;

		//initialize vao and vbo of the object;


		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, data_size*(4+4+3+2), NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, data_size*4, v_positions );
		glBufferSubData( GL_ARRAY_BUFFER, data_size*4, data_size*4, v_colors );
		glBufferSubData( GL_ARRAY_BUFFER, data_size*(4+4), data_size*3, v_normals );
		glBufferSubData( GL_ARRAY_BUFFER, data_size*(4+4+3), data_size*2, v_texcord );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(data_size*4));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(data_size*4+data_size*4));

		glEnableVertexAttribArray( vTexcord );
		glVertexAttribPointer( vTexcord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(data_size*4+data_size*4+data_size*3));


		
		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=0;

		update_matrices();
	}

	void HNode::update_matrices(){

		rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
		rotation = glm::rotate(rotation, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));


	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);

	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;

		update_matrices();
	}

	void HNode::render(){

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		normal_matrix = glm::transpose (glm::inverse(glm::mat3(*ms_mult)));
		glUniformMatrix3fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glBindVertexArray (vao);

		if(this->use_texture)
		{
			glBindTexture(GL_TEXTURE_2D, tex);
			glUniform1f(vUseTexture,1.0f);	
		}
		else
		{
			glUniform1f(vUseTexture,-1.0f);	
		}
		
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory 
		delete ms_mult;

	}

	void HNode::render_tree(){
		
		matrixStack.push_back(translation);
		matrixStack.push_back(rotation);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}
		matrixStack.pop_back();
		matrixStack.pop_back();

	}

	void HNode::inc_rx(){
		rx+=10;	
		update_matrices();
	}


	void HNode::inc_ry(){
		ry+=10;
		update_matrices();
	}

	void HNode::inc_rz(){
		rz+=10;
		update_matrices();
	}

	void HNode::dec_rx(){
		rx-=10;
		update_matrices();
	}

	void HNode::dec_ry(){
		ry-=10;
		update_matrices();
	}

	void HNode::dec_rz(){
		rz-=10;
		//std::cout<<rz<<std::endl;
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}