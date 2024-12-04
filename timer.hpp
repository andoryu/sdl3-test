#pragma once

#include <SDL3/SDL.h>


class Timer
{
    Uint64 previous_ticks;
public:
    float elapsed_seconds;

public:
    void tick(void);
};
