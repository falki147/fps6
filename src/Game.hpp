#pragma once

#include <Scenes/Scene.hpp>
#include <SDL_keycode.h>
#include <SDL_scancode.h>

class ResourceHandler;

namespace Game {
	void Init();
	void Update(float delta);
	void Render();
	void Cleanup();
	void Goto(Scene* scene);
	void Leave();
	void keyDown(SDL_Scancode code, SDL_Keycode key);
	void keyUp(SDL_Scancode code, SDL_Keycode key);
	ResourceHandler* getResourceHandler();
}
