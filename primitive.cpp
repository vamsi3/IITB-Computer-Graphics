#include "primitive.hpp"
#include <cmath>

#include <iostream>

namespace cs475 {

	Sphere* Sphere::set(double radius1,double radius2,double radius3, double alpha_phi=0, double beta_phi=2*M_PI, double alpha_theta=0, double beta_theta=2*M_PI, double precision=360, bool center = true) {
		this->num_vertices = 0;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->radius1 = radius1;
		this->radius2 = radius2;
		this->radius3 = radius3;
		this->alpha_phi = alpha_phi;
		this->beta_phi = beta_phi;
		this->alpha_theta = alpha_theta;
		this->beta_theta = beta_theta;
		this->precision = precision;
		this->center = center;
		return this;
	}

	glm::vec4 Sphere::sphericalToCartesian(const double radius1,const double radius2,const double radius3, const double theta, const double phi) const {

		if(this->center)
		{
			return glm::vec4(radius1*cos(phi)*cos(theta), radius2*cos(phi)*sin(theta), radius3*sin(phi), 1.0); 
		}
		else
		{
			return glm::vec4(radius1*cos(phi)*cos(theta), radius2+radius2*cos(phi)*sin(theta), radius3*sin(phi), 1.0); 
		}
		
	}

	Sphere* Sphere::load() {
		double half_delta_theta = ((double) M_PI) / this->precision;
		double delta_theta = 2.0*half_delta_theta;
		double delta_phi = (this->beta_phi - this->alpha_phi) / this->precision;
		double initial_theta = this->alpha_theta;

		for (double phi = this->alpha_phi; phi <= this->beta_phi-delta_phi; phi += delta_phi) {
			for (double theta = initial_theta; theta <= this->beta_theta; theta += delta_theta) {
				this->addVertex(radius1, radius2, radius3, theta, phi);
				this->addVertex(radius1, radius2, radius3, theta+half_delta_theta, phi+delta_phi);
				this->addVertex(radius1, radius2, radius3, theta+delta_theta, phi);				
				this->addVertex(radius1, radius2, radius3, theta-half_delta_theta, phi+delta_phi);
				this->addVertex(radius1, radius2, radius3, theta, phi);
				this->addVertex(radius1, radius2, radius3, theta+half_delta_theta, phi+delta_phi);
			}
			if (initial_theta == alpha_theta) initial_theta += half_delta_theta;
			else initial_theta -= half_delta_theta;
		}

		return this;
	}

	void Sphere::addVertex(const double radius1,const double radius2,const double radius3, const double theta, const double phi) {
		v_positions[this->num_vertices] = sphericalToCartesian(radius1, radius2, radius3, theta, phi);
		v_colors[this->num_vertices] = this->color;
		if(this->center) v_normals[this->num_vertices] = glm::vec3(radius1*cos(phi)*cos(theta), radius2*cos(phi)*sin(theta), radius3*sin(phi)); 
		v_normals[this->num_vertices] = glm::vec3(radius1*cos(phi)*cos(theta), radius2+radius2*cos(phi)*sin(theta), radius3*sin(phi)); 
		v_texcord[this->num_vertices] = glm::vec2(theta/(2*M_PI), 0.5f + phi/M_PI);
		this->num_vertices++;
	}

	// Cone

	Cone* Cone::set(double alpha_radius, double beta_radius, double alpha_z, double beta_z, double alpha_theta, double beta_theta, double precision_theta) {
		this->num_vertices = 0;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->alpha_radius = alpha_radius;
		this->beta_radius = beta_radius;
		this->alpha_z = alpha_z;
		this->beta_z = beta_z;
		this->alpha_theta = alpha_theta;
		this->beta_theta = beta_theta;
		this->precision_theta = precision_theta;
		return this;
	}

	glm::vec4 Cone::cylindricalToCartesian(const double radius, const double theta, const double z) const {
		return glm::vec4(radius*cos(theta), radius*sin(theta), z, 1.0);
	}

	Cone* Cone::load() {
		double delta_theta = (this->beta_theta - this->alpha_theta) / this->precision_theta;
		double half_delta_theta = delta_theta / 2;
		double bradius = this->alpha_radius;
		double uradius = this->beta_radius;
		double initial_theta = this->alpha_theta;

		for (double theta = initial_theta; theta <= this->beta_theta - delta_theta; theta += delta_theta) {
			this->addVertex(bradius, theta, alpha_z);
			this->addVertex(uradius, theta, beta_z);
			this->addVertex(uradius, theta+delta_theta, beta_z);			
			this->addVertex(uradius, theta+delta_theta, beta_z);
			this->addVertex(bradius, theta, alpha_z);
			this->addVertex(bradius, theta+delta_theta, alpha_z);
		}

			
		return this;
	}

	void Cone::addVertex(double radius, double theta, double z) {
		v_positions[this->num_vertices] = cylindricalToCartesian(radius, theta, z);
		v_colors[this->num_vertices] = this->color;
		v_normals[this->num_vertices] = glm::vec3(radius*cos(theta), radius*sin(theta), 0.0);
		v_texcord[this->num_vertices] = glm::vec2(theta/(2*M_PI), z/(this->beta_z - this->alpha_z));
		this->num_vertices++;
	}

	// Cuboid

	Cuboid* Cuboid::set(double alpha_x, double beta_x, double alpha_y, double beta_y, double alpha_z, double beta_z) {
		this->num_vertices = 0;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->alpha_x = alpha_x;
		this->beta_x = beta_x;
		this->alpha_y = alpha_y;
		this->beta_y = beta_y;
		this->alpha_z = alpha_z;
		this->beta_z = beta_z;

		return this;
	}

	Cuboid* Cuboid::load(int t) {

		if(t == 0)
		{
			glm::vec4 v0(alpha_x, alpha_y, beta_z, 1.0);
			glm::vec4 v1(alpha_x, beta_y, beta_z, 1.0);
			glm::vec4 v2(beta_x, beta_y, beta_z, 1.0);
			glm::vec4 v3(beta_x, alpha_y, beta_z, 1.0);
			glm::vec4 v4(alpha_x, alpha_y, alpha_z, 1.0);
			glm::vec4 v5(alpha_x, beta_y, alpha_z, 1.0);
			glm::vec4 v6(beta_x, beta_y, alpha_z, 1.0);
			glm::vec4 v7(beta_x, alpha_y, alpha_z, 1.0);

			Quad* quad = new Quad(); quad->setColor(this->color);
			quad->set(v1, v0, v3, v2)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v2, v3, v7, v6)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v3, v0, v4, v7)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v6, v5, v1, v2)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v4, v5, v6, v7)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v5, v4, v0, v1)->load(num_vertices); this->num_vertices += quad->getVertexCount();

		}
	   	else
	   	{
	   		glm::vec4 v0(alpha_x, alpha_y, 0, 1.0);
			glm::vec4 v1(alpha_x, 0, 0, 1.0);
			glm::vec4 v2(0, 0, 0, 1.0);
			glm::vec4 v3(0, alpha_y, 0, 1.0);
			glm::vec4 v4(alpha_x, alpha_y, alpha_z, 1.0);
			glm::vec4 v5(alpha_x, 0, alpha_z, 1.0);
			glm::vec4 v6(0, 0, alpha_z, 1.0);
			glm::vec4 v7(0, alpha_y, alpha_z, 1.0);

			Quad* quad = new Quad(); quad->setColor(this->color);
			quad->set(v1, v0, v3, v2)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v2, v3, v7, v6)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v3, v0, v4, v7)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v6, v5, v1, v2)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v4, v5, v6, v7)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v5, v4, v0, v1)->load(num_vertices); this->num_vertices += quad->getVertexCount();

	   	}

	    
		return this;
	}

	// Trapezoid

	Trapezoid* Trapezoid::set(double alpha_x, double beta_x, double alpha_y, double beta_y, double alpha_z, double beta_z) {
		this->num_vertices = 0;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->alpha_x = alpha_x;
		this->beta_x = beta_x;
		this->alpha_y = alpha_y;
		this->beta_y = beta_y;
		this->alpha_z = alpha_z;
		this->beta_z = beta_z;

		return this;
	}

	Trapezoid* Trapezoid::load(int t) {
		glm::vec4 v0(alpha_x, alpha_y, alpha_z, 1.0);
		glm::vec4 v1(-alpha_x, alpha_y, alpha_z, 1.0);
		glm::vec4 v2(-alpha_x, -alpha_y, alpha_z, 1.0);
		glm::vec4 v3(alpha_x, -alpha_y, alpha_z, 1.0);
		glm::vec4 v4(beta_x, beta_y, beta_z, 1.0);
		glm::vec4 v5(-beta_x, beta_y, beta_z, 1.0);
		glm::vec4 v6(-beta_x, -beta_y, beta_z, 1.0);
		glm::vec4 v7(beta_x, -beta_y, beta_z, 1.0);

		Quad* quad = new Quad(); quad->setColor(this->color);
		quad->set(v0, v4, v7, v3)->load(num_vertices); this->num_vertices += quad->getVertexCount();
		quad->set(v1, v5, v4, v0)->load(num_vertices); this->num_vertices += quad->getVertexCount();
		quad->set(v2, v6, v5, v1)->load(num_vertices); this->num_vertices += quad->getVertexCount();
		quad->set(v3, v7, v6, v2)->load(num_vertices); this->num_vertices += quad->getVertexCount();

		if (t == 1) {
			quad->set(v0, v1, v2, v3)->load(num_vertices); this->num_vertices += quad->getVertexCount();
			quad->set(v4, v5, v6, v7)->load(num_vertices); this->num_vertices += quad->getVertexCount();
		}

	    
		return this;
	}

	// Quad

	Quad* Quad::set(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d) {
		this->num_vertices = 0;
		//this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->quad_normal = glm::cross(glm::vec3(b-a), glm::vec3(c-a));

		return this;
	}

	Quad* Quad::load(GLuint offset) {
		v_texcord[num_vertices+offset] = glm::vec2(0, 0); this->addVertex(a, offset);
		v_texcord[num_vertices+offset] = glm::vec2(1, 0); this->addVertex(b, offset);
		v_texcord[num_vertices+offset] = glm::vec2(1, 1); this->addVertex(c, offset);
		v_texcord[num_vertices+offset] = glm::vec2(0, 0); this->addVertex(a, offset);
		v_texcord[num_vertices+offset] = glm::vec2(1, 1); this->addVertex(c, offset);
		v_texcord[num_vertices+offset] = glm::vec2(0, 1); this->addVertex(d, offset);

		return this;
	}

	void Quad::addVertex(const glm::vec4 &x, GLuint offset) {
		v_positions[num_vertices+offset] = x;
		v_colors[num_vertices+offset] = this->color;
		v_normals[num_vertices+offset] = this->quad_normal;
		this->num_vertices++;
	}

	// Disc

	Disc* Disc::set(double radius, double alpha_theta, double beta_theta, double precision_theta) {
		this->num_vertices = 0;
		this->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
		this->radius = radius;
		this->alpha_theta = alpha_theta;
		this->beta_theta = beta_theta;
		this->precision_theta = precision_theta;

		return this;
	}

	Disc* Disc::load() {
		double delta_theta = (beta_theta - alpha_theta) / precision_theta;

		for (double theta=alpha_theta; theta < beta_theta; theta += delta_theta) {
			v_positions[num_vertices] = glm::vec4(0.0, 0.0, 0.0, 1.0);
			v_colors[num_vertices] = this->color;
			v_normals[num_vertices] = glm::vec3(0.0, 0.0, 1.0);
			v_texcord[num_vertices] = glm::vec2(0.5,0.5);
			this->num_vertices++;
			v_positions[num_vertices] = glm::vec4(radius*cos(theta), radius*sin(theta), 0.0, 1.0);
			v_colors[num_vertices] = this->color;
			v_normals[num_vertices] = glm::vec3(0.0, 0.0, 1.0);
			v_texcord[num_vertices] = glm::vec2((1+cos(theta))/2, (1+sin(theta))/2);
			this->num_vertices++;
			v_positions[num_vertices] = glm::vec4(radius*cos(theta+delta_theta), radius*sin(theta+delta_theta), 0.0, 1.0);
			v_colors[num_vertices] = this->color;
			v_normals[num_vertices] = glm::vec3(0.0, 0.0, 1.0);
			v_texcord[num_vertices] = glm::vec2((1+cos(theta+delta_theta))/2, (1+sin(theta+delta_theta))/2);
			this->num_vertices++;
		}

		return this;
	}

}
