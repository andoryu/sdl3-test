#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 frag_colour;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main() {
    gl_Position = projection_matrix * model_matrix * vec4(position, 1.0);
    frag_colour = colour;
}
