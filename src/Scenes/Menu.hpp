#pragma once

#include <Scenes/Scene.hpp>
#include <Program/Sprite.hpp>
#include <Graphics/StaticBatch.hpp>
#include <Math/Vector.hpp>

class MenuScene: public Scene {
public:
	MenuScene();

	void update();
	void render();
	void onKeyDown(SDL_Scancode code, SDL_Keycode key);
	void onKeyUp(SDL_Scancode code, SDL_Keycode key);

private:
	struct Vertex {
		vec3 pos;
		vec2 texcoord;
	};

	void renderMenuSprite(unsigned int ind, vec2 pos);

	SpriteProgram m_prog;
	StaticBatch<Vertex> m_batch;
};
