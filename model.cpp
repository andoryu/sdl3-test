#include "model.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "appstate.hpp"
#include "shader.hpp"
#include <iostream>


void shape_create(Model *model, std::vector<Vertex> vertices, std::vector<GLushort> indices, GLuint shader_program)
{
    model->m_vertices = vertices;
    model->m_indices  = indices;
    model->m_shader_program = shader_program;

    model->vao = 0;
    model->vbo = 0;
    model->ebo = 0;

    shape_prepare(model);
}

void shape_prepare(Model *model)
{
    glGenVertexArrays(1, &model->vao);
    glGenBuffers(1, &model->vbo);
    glGenBuffers(1, &model->ebo);

    glBindVertexArray(model->vao);

    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, model->m_vertices.size() * sizeof(Vertex), model->m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->m_indices.size() * sizeof(GLushort), model->m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colour));

    glBindVertexArray(0);

    std::cout << "Finished prepare" << std::endl;
}

void shape_draw(Model *model) {

    glUseProgram(model->m_shader_program);
    glBindVertexArray(model->vao);
    glDrawElements(GL_TRIANGLES, model->m_indices.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
