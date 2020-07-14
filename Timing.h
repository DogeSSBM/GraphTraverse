#pragma once
#define FPS		60
#define TPF		17

static inline
Ticks getTicks(void)
{
	return SDL_GetTicks();
}
