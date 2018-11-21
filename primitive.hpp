#ifndef _PRIMITIVE_HPP_
#define _PRIMITIVE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"
#include "defs.hpp"

namespace cs475	 {

	class Primitive {
	protected:
		GLuint num_vertices;
		glm::vec4 color;
	
	public:
		GLuint getVertexCount() const { return num_vertices; }
		void setColor(double r, double g, double b, double a) { this->color = glm::vec4(r, g, b ,a); }
	};

	// 3D Primitives

	class Sphere : public Primitive {
		double radius1,radius2,radius3, alpha_phi, beta_phi, alpha_theta, beta_theta, precision;
		bool center;

		glm::vec4 sphericalToCartesian(const double radius1, const double radius2, const double radius3, const double theta, const double phi) const;
		void addVertex(double, double, double, double, double);

	public:
		Sphere* set(double radius1, double radius2, double radius3, double alpha_phi, double beta_phi, double alpha_theta, double beta_theta, double precision, bool center);
		Sphere* load();
		Sphere* setColor(double r, double g, double b, double a) { Primitive::setColor(r, g, b, a); return this; }
	};

	class Cone : public Primitive {
		double alpha_radius, beta_radius, alpha_z, beta_z, alpha_theta, beta_theta, precision_theta;

		glm::vec4 cylindricalToCartesian(const double radius, const double theta, const double z) const;
		void addVertex(double, double, double);

	public:
		Cone* set(double alpha_radius, double beta_radius, double alpha_z, double beta_z, double alpha_theta, double beta_theta, double precision_theta);
		Cone* load();
		Cone* setColor(double r, double g, double b, double a) { Primitive::setColor(r, g, b, a); return this; }
	};

	class Cuboid : public Primitive {
		double alpha_x, beta_x, alpha_y, beta_y, alpha_z, beta_z;

	public:
		Cuboid* set(double alpha_x, double beta_x, double alpha_y, double beta_y, double alpha_z, double beta_z);
		Cuboid* load(int t);
		Cuboid*	setColor(double r, double g, double b, double a) { Primitive::setColor(r, g, b, a); return this; }
	};

	class Trapezoid : public Primitive {
		double alpha_x, beta_x, alpha_y, beta_y, alpha_z, beta_z;

	public:
		Trapezoid* set(double alpha_x, double beta_x, double alpha_y, double beta_y, double alpha_z, double beta_z);
		Trapezoid* load(int t);
		Trapezoid*	setColor(double r, double g, double b, double a) { Primitive::setColor(r, g, b, a); return this; }
	};

	// 2D Primitives

	class Quad : public Primitive {
		glm::vec4 a, b, c, d;
		glm::vec3 quad_normal;

		void addVertex(const glm::vec4&, GLuint);

	public:
		Quad* set(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
		Quad* load(GLuint offset);
		Quad* setColor(glm::vec4 color) { this->color = color; return this; }
	};

	class Disc : public Primitive {
		double radius, alpha_theta, beta_theta, precision_theta;

	public:
		Disc* set(double radius, double alpha_theta, double beta_theta, double precision_theta);
		Disc* load();
		Disc* setColor(double r, double g, double b, double a) { Primitive::setColor(r, g, b, a); return this; }
	};

};

#endif