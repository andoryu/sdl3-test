project('sdl3-test', ['cpp', 'c'], version: '0.1')

libsdl3_deps = dependency('sdl3')
libglm_deps = dependency('glm', method: 'cmake')

c_flags = ['-std=c99', '-O0']
cpp_flags = ['-std=c++20', '-O0']

glad_src = ['glad/gl.c']

app_src = ['main.cpp', 'model.cpp', 'shader.cpp', 'timer.cpp']

executable(
  'sdl3-test',
  [
    app_src,
    glad_src,
  ],
  dependencies: [libsdl3_deps, libglm_deps],
  c_args: c_flags,
  cpp_args: cpp_flags,
)
