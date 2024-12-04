#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <string>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/gl.h"

#include "shader.hpp"
#include "model.hpp"
#include "timer.hpp"
#include "appstate.hpp"


SDL_AppResult initialise_sdl(void)
{
    SDL_SetAppMetadata("sdl3_test", "0.1", "gumbinaut");

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        const char *err = SDL_GetError();
        std::cerr << "Failed to initialise SDL: " << err << std::endl;
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void set_attributes(void)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
}

SDL_AppResult setup_window(AppState *app) {
    SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    app->window = SDL_CreateWindow(app->title.c_str(), app->width, app->height, flags);
    if (!app->window) {
        auto err = SDL_GetError();
        std::cerr << "Failed to create window: " << err << std::endl;
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult setup_opengl(AppState *app) {
    app->context = SDL_GL_CreateContext(app->window);
    if (!app->context) {
        auto err = SDL_GetError();
        std::cerr << "Failed to create context: " << err << std::endl;
        return SDL_APP_FAILURE;
    }

    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    SDL_GL_SetSwapInterval(1);

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    SDL_AppResult result;

    AppState *app = new AppState{
        800, 600,
        "this is not a test!"
    };

    result = initialise_sdl();
    if(result != SDL_APP_CONTINUE)
    {
        return result;
    }

    set_attributes();

    result = setup_window(app);
    if(result != SDL_APP_CONTINUE)
    {
        return result;
    }

    result = setup_opengl(app);
    if(result != SDL_APP_CONTINUE)
    {
        return result;
    }

    app->vertex_shader = create_shader(GL_VERTEX_SHADER, "../assets/shaders/shader.vert");
    app->frag_shader   = create_shader(GL_FRAGMENT_SHADER, "../assets/shaders/shader.frag");

    app->shader_program = create_shader_program(app->vertex_shader, app->frag_shader);

    glm::mat4 projection = glm::ortho<float>(0.0f,
        (float)app->width, (float)app->height,
        0.0f, -0.5f, 100.0f);
    load_projection_matrix(app->shader_program, projection);

    glViewport(0, 0, app->width, app->height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<Vertex> vertices{
        {{100.0f, 100.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{200.0f, 100.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{200.0f, 200.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{100.0f, 200.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    };

    std::vector<GLushort> indices{
        0, 1, 2,
        2, 3, 0,
    };

    shape_create(&app->shape, vertices, indices, app->shader_program);


    glm::mat4 model(1.0f);
    model = glm::translate(model, {0.0f, 0.0f, -5.0f});
    load_model_matrix(app->shader_program, model);

    app->last_frame = SDL_GetTicks();

    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app = (AppState *)appstate;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::ortho<float>(0.0f, (float)app->width, (float)app->height, 0.0f, 0.01f, 100.0f);
    load_projection_matrix(app->shader_program, projection);
    glViewport(0, 0, app->width, app->height);

    shape_draw(&app->shape);

    SDL_GL_SwapWindow(app->window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app = (AppState *)appstate;

    switch(event->type)
    {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_UP:
        {
            SDL_Keycode key = event->key.key;
            if(key == 'q' || key == 'Q')
            {
                return SDL_APP_SUCCESS;
            }
        }

        case SDL_EVENT_WINDOW_RESIZED:
        {
            int newWidth = event->window.data1;
            int newHeight = event->window.data2;

            std::cout << "Resized: Width, height: " << newWidth << ", " << newHeight << std::endl;

            // glm::mat4 projection = glm::ortho<float>(0.0f, (float)newWidth, (float)newHeight, 0.0f, 0.01f, 100.0f);
            // load_projection_matrix(app->shader_program, projection);
            // glViewport(0, 0, newWidth, newHeight);

            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // shape_draw(&app->shape);
            // SDL_GL_SwapWindow(app->window);

            app->width = newWidth;
            app->height = newHeight;
            return SDL_APP_CONTINUE;
        }

        case SDL_EVENT_WINDOW_EXPOSED:
        {
            int newWidth = event->window.data1;
            int newHeight = event->window.data2;

            // std::cout << "Exposed: Width, height: " << newWidth << ", " << newHeight << std::endl;
        }
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *app = (AppState *)appstate;

    SDL_GL_DestroyContext(app->context);
    SDL_DestroyWindow(app->window);
    delete(app);

    SDL_Quit();
}
