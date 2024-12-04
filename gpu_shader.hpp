#pragma once

#include <SDL3/SDL.h>

#include <string>

SDL_GPUShader* gpu_vertex_shader_create(SDL_GPUDevice *device, std::string path);
SDL_GPUShader* gpu_fragment_shader_create(SDL_GPUDevice *device, std::string path);
