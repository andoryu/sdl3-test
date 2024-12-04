#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "glad/gl.h"

GLuint create_shader(GLenum shader_type, const char *path);
GLuint create_shader_program(GLuint vertex, GLuint fragment);

void load_matrix(GLuint shader_program, glm::mat4 matrix, const char *uniform_name);
void load_projection_matrix(GLuint shader_program, glm::mat4 matrix);
void load_model_matrix(GLuint shader_program, glm::mat4 matrix);
std::vector<char> load_shader(const char *file_name);
