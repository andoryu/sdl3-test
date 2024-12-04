#include "shader.hpp"

#include <iostream>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>

#include "glad/gl.h"

std::vector<char> load_shader(const char *file_name) {
    std::ifstream file{file_name, std::ios::ate};

    size_t file_size = file.tellg();
    std::vector<char> buffer(file_size+1);

    file.seekg(0);
    file.read(buffer.data(), file_size);

    file.close();

    buffer.push_back('\0');

    return buffer;
}


GLuint create_shader(GLenum shader_type, const char *path) {
    std::vector<char> code = load_shader(path);
    auto data = code.data();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    int info_log_length;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

    if (!result) {
        std::vector<char> message(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, nullptr, &message[0]);

        std::cout << "Shader: " << path << std::endl;
        std::cout << message.data() << std::endl;
    }

    return shader;
}

GLuint create_shader_program(GLuint vertex, GLuint fragment)
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    glDetachShader(program, fragment);
    glDetachShader(program, vertex);

    return program;
}

void load_matrix(GLuint shader_program, glm::mat4 matrix, const char *uniform_name) {
    glUseProgram(shader_program);
    GLuint location = glGetUniformLocation(shader_program, uniform_name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
    glUseProgram(0);
}


void load_projection_matrix(GLuint shader_program, glm::mat4 matrix) {
    load_matrix(shader_program, matrix, "projection_matrix");
}

void load_model_matrix(GLuint shader_program, glm::mat4 matrix) {
    load_matrix(shader_program, matrix, "model_matrix");
}
