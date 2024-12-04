#include "gpu_shader.hpp"

#include <SDL3/SDL.h>
#include <string>



SDL_GPUShader* gpu_vertex_shader_create(SDL_GPUDevice *device, std::string path)
{
    load_compiled_shader(path);
}
SDL_GPUShader* gpu_fragment_shader_create(SDL_GPUDevice *device, std::string path)
{

}
