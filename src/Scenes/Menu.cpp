#include <Scenes/Menu.hpp>
#include <Game.hpp>
#include <ResourceHandler.hpp>
#include <Window.hpp>
#include <Level.hpp>
#include <Scenes/Main.hpp>
#include <ctime>

MenuScene::MenuScene() {
	m_batch.beginAttributes();
	m_batch.attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	m_batch.attribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 12);
	m_batch.endAttributes();

	renderMenuSprite(2, vec2(0, 0));
	renderMenuSprite(0, vec2(20, 370));
	renderMenuSprite(1, vec2(20, 480));

	m_batch.upload();
}

void MenuScene::update() {
	if (Window::wasMouseReleased()) {
		auto x = Window::mouseX(), y = Window::mouseY();

		if (x >= 20 && x <= 320 && y >= 370 && y <= 470)
			Game::Goto(new MainScene(Level(Game::getResourceHandler()->getLevel())));
		else if (x >= 20 && x <= 320 && y >= 480 && y <= 580) {
			std::srand((unsigned int) std::time(nullptr));
			auto grid = Dungeon::create({ 21, 21, 50, 2, 0.7f, 0.01f });
			Game::Goto(new MainScene(Level(grid)));
		}
	}
}

void MenuScene::render() {
	m_prog.use();
	m_prog.setMVP(mat4::createOrthographic(0, 800, 600, 0, -1, 1));

	Game::getResourceHandler()->getSprite()[SpriteCollection::SpriteMenu].bindStage(0);
	
	glDisable(GL_DEPTH_TEST);
	m_batch.render();
	glEnable(GL_DEPTH_TEST);
}

void MenuScene::onKeyDown(SDL_Scancode code, SDL_Keycode key) { }

void MenuScene::onKeyUp(SDL_Scancode code, SDL_Keycode key) { }

void MenuScene::renderMenuSprite(unsigned int ind, vec2 pos) {
	auto spr = Game::getResourceHandler()->getSprite().getInfo(SpriteCollection::SpriteMenu, ind);

	auto p1 = pos + vec2(spr.offset.x, spr.offset.y);
	auto p2 = p1 + vec2(spr.size.x, spr.size.y);

	m_batch.add({ vec3(p1.x, p1.y, 0), vec2(spr.p1.x, spr.p1.y) });
	m_batch.add({ vec3(p1.x, p2.y, 0), vec2(spr.p1.x, spr.p2.y) });
	m_batch.add({ vec3(p2.x, p1.y, 0), vec2(spr.p2.x, spr.p1.y) });
	m_batch.add({ vec3(p2.x, p1.y, 0), vec2(spr.p2.x, spr.p1.y) });
	m_batch.add({ vec3(p2.x, p2.y, 0), vec2(spr.p2.x, spr.p2.y) });
	m_batch.add({ vec3(p1.x, p2.y, 0), vec2(spr.p1.x, spr.p2.y) });
}
