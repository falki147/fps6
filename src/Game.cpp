#include <Game.hpp>
#include <glad/glad.h>
#include <stack>
#include <Scenes/Scene.hpp>
#include <Scenes/Main.hpp>
#include <fstream>
#include <ctime>
#include <memory>
#include <ResourceHandler.hpp>
#include <Scenes/Menu.hpp>

static auto g_scenes = std::stack<Scene*>();
static auto g_resources = std::unique_ptr<ResourceHandler>();
static auto g_leave = false;

void Game::Init() {
	IMG img("data.img");
	g_resources.reset(new ResourceHandler(img));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Goto(new MenuScene());

	/*std::srand((unsigned int) std::time(nullptr));
	auto grid = Dungeon::create({ 21, 21, 50, 2, 0.7f, 0.01f });
	Goto(new MainScene(Level(grid), img));*/

	// Goto(new MainScene(Level(img["level.dat"]), img));
}

void Game::Update(float delta) {
	if (!g_scenes.empty()) {
		g_scenes.top()->update();

		if (g_leave) {
			g_leave = false;
			delete g_scenes.top();
			g_scenes.pop();
		}
	}
}

void Game::Render() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!g_scenes.empty())
		g_scenes.top()->render();
}

void Game::Cleanup() {
	while (!g_scenes.empty()) {
		delete g_scenes.top();
		g_scenes.pop();
	}

	g_resources.release();
}

void Game::Goto(Scene* scene) {
	g_scenes.push(scene);
}

void Game::Leave() {
	g_leave = true;
}

void Game::keyDown(SDL_Scancode code, SDL_Keycode key) {
	if (!g_scenes.empty())
		g_scenes.top()->onKeyDown(code, key);
}

void Game::keyUp(SDL_Scancode code, SDL_Keycode key) {
	if (!g_scenes.empty())
		g_scenes.top()->onKeyUp(code, key);
}

ResourceHandler* Game::getResourceHandler() {
	return g_resources.get();
}
