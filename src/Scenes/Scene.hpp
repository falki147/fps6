#pragma once

#include <SDL_keycode.h>

class Scene {
public:
	virtual ~Scene() { }

	virtual void update() = 0;
	virtual void render() = 0;
	virtual void onKeyDown(SDL_Scancode code, SDL_Keycode key) = 0;
	virtual void onKeyUp(SDL_Scancode code, SDL_Keycode key) = 0;
};
