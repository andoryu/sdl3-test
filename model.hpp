#pragma once

#include <glm/glm.hpp>

#include "glad/gl.h"

struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec3 colour{0.0f};
    // glm::vec2 tex_coord;
};

struct Model {
    std::vector<Vertex> m_vertices;
    std::vector<GLushort> m_indices;
    GLuint m_shader_program = 0;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

void shape_draw(Model *model);
void shape_prepare(Model *model);
void shape_create(Model *model, std::vector<Vertex> vertices, std::vector<GLushort> indices, GLuint shader_program);
