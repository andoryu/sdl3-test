# SDL3-Test
Simple "hello square" testcode.

## Build instructions
this uses meson so in this directory:
- mkdir build
- meson setup build/
- cd build
- meson compile
- ./sdl3-test

try vertically resizing the window.

then change the **if** statement on line 176 in SDL_AppIterate to **if(true) {...} ** and try vertically resizing window.

