#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include "model.hpp"

// #include "model.hpp"

struct AppState
{
    int width, height;
    std::string title;

    SDL_Window *window;
    SDL_GLContext context;

    GLuint vertex_shader;
    GLuint frag_shader;
    GLuint shader_program;

    Model shape;

    Uint64 last_frame;
};


struct AppStateGPU
{
    int width, height;
    std::string title;

    SDL_Window *window;
    SDL_GPUDevice *device;
};
