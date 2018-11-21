#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const int num_vertices = 1000000;
extern glm::vec4 v_positions[num_vertices];
extern glm::vec4 v_colors[num_vertices];
extern glm::vec3 v_normals[num_vertices];
extern glm::vec2 v_texcord[num_vertices];
