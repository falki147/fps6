#include <Entities/Player.hpp>
#include <Scenes/Main.hpp>
#include <SDL.h>
#include <Window.hpp>
#include <Game.hpp>

static unsigned char mix(unsigned char a, unsigned char b, float amount) {
	return (unsigned char) (a * (1 - amount) + b * amount);
}

Player::Player(vec2 pos): m_position(pos) {
	m_object = Collision::createCircle({ m_position, 8 });
}

void Player::update(MainScene& scene) {
	m_moved = false;

	auto state = SDL_GetKeyboardState(nullptr);
	auto speed = 1.0f;

	if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
		speed = 2.0f;

	if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
		m_position.x += speed * std::cos(m_direction);
		m_position.y -= speed * std::sin(m_direction);
		m_object.circle = { m_position, 8 };
		m_moved = true;
	}

	if (scene.getCheatStatus()["HESOYAM"]) {
		scene.getCheatStatus()["HESOYAM"] = false;
		m_health = 200;
	}

	if (scene.getCheatStatus()["BAGOFIX"]) {
		m_health = 100.0f;
	}
	
	if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
		m_position.x -= speed * std::cos(m_direction);
		m_position.y += speed * std::sin(m_direction);
		m_object.circle = { m_position, 8 };
		m_moved = true;
	}

	if (state[SDL_SCANCODE_A]) {
		m_position.x += speed * std::sin(m_direction);
		m_position.y += speed * std::cos(m_direction);
		m_object.circle = { m_position, 8 };
		m_moved = true;
	}

	if (state[SDL_SCANCODE_D]) {
		m_position.x -= speed * std::sin(m_direction);
		m_position.y -= speed * std::cos(m_direction);
		m_object.circle = { m_position, 8 };
		m_moved = true;
	}

	m_direction += Window::mouseRelativeX() * 0.01f;

	if (state[SDL_SCANCODE_LEFT])
		m_direction -= 0.2f;

	if (state[SDL_SCANCODE_RIGHT])
		m_direction += 0.2f;

	if (m_shootingAnimation) {
		m_index += 0.3f;

		auto numImages = scene.getCheatStatus()["CHAIN"] ? 2 : (scene.getCheatStatus()["FORK"] ? 4 : 11);

		if (m_index > numImages) {
			m_index = 0;
			m_shootingAnimation = false;
		}
	}
	
	if (!m_shootingAnimation && (state[SDL_SCANCODE_SPACE] || Window::isMouseDown())) {
		auto& sound = Game::getResourceHandler()->getSound()[SoundCollection::SoundShot];
		sound.stop();
		sound.play();

		m_shootingAnimation = true;
		m_index = 0.3f;

		auto minDist = scene.getCheatStatus()["FORK"] ? 50.f : 200.f;
		Entity* entityHit = nullptr;
			
		Collision::Ray ray = {
			m_position,
			vec2(std::cos(m_direction), -std::sin(m_direction))
		};

		for (auto line : scene.getCollisionLines()) {
			auto dist = Collision::cast(ray, line);

			if (dist >= 0 && dist < minDist)
				minDist = dist;
		}

		for (auto entity : scene.getEntities()) {
			if (!entity->isShootable())
				continue;

			auto dist = Collision::cast(ray, entity->getObject());

			if (dist >= 0 && dist < minDist) {
				entityHit = entity;
				minDist = dist;
			}
		}

		if (entityHit)
			entityHit->hit(scene);
	}
}

void Player::render(MainScene& scene) {
	scene.setCamera(
		vec3(m_position.x, m_position.y, m_z),
		vec3(m_position.x + std::cos(m_direction), m_position.y - std::sin(m_direction), m_z),
		vec3(0, 0, 1)
	);

	if (scene.getCheatStatus()["CHAIN"])
		scene.renderSprite(SpriteCollection::SpriteChaingun, (unsigned int) m_index + (m_shootingAnimation ? 1 : 0), vec2(206, 314), vec2(2, 2));
	else if (scene.getCheatStatus()["FORK"])
		scene.renderSprite(SpriteCollection::SpriteFork, (unsigned int) m_index, vec2(247, 212), vec2(2, 2));
	else
		scene.renderSprite(SpriteCollection::SpriteGun, (unsigned int) m_index, vec2(0, 224), vec2(2, 2));
	
	const auto alpha = 0x66000000;

	scene.renderRectangle(vec2(5, 460), vec2(100, 461), alpha);
	scene.renderRectangle(vec2(5, 474), vec2(100, 475), alpha);
	scene.renderRectangle(vec2(5, 460), vec2(6, 475), alpha);
	scene.renderRectangle(vec2(99, 460), vec2(100, 475), alpha);

	auto health = std::min(100.f, m_health);
	auto r = health < 50 ? mix(0, 0xFF, health / 50) : mix(0xFF, 0, health / 50 - 1);
	auto g = health < 50 ? 0 : mix(0, 0xFF, health / 50 - 1);
	auto color = alpha | (g << 8) | r;

	scene.renderRectangle(vec2(6, 461), vec2(6 + health * 93 / 100, 474), color);

	if (m_health > 100) {
		health = m_health - 100;

		scene.renderRectangle(vec2(5, 435), vec2(100, 436), alpha);
		scene.renderRectangle(vec2(5, 449), vec2(100, 450), alpha);
		scene.renderRectangle(vec2(5, 435), vec2(6, 450), alpha);
		scene.renderRectangle(vec2(99, 435), vec2(100, 450), alpha);
		scene.renderRectangle(vec2(6, 436), vec2(6 + health * 93 / 100, 449), alpha | 0xFF00);
	}
}

void Player::setPosition(vec2 pos) {
	m_position = pos;
	m_object.circle = { m_position, 8 };
}

void Player::setHealth(float health) {
	m_health = health;

	if (m_health <= 0) {
		SDL_ShowSimpleMessageBox(0, "fps6", "You dieded!", nullptr);
		Window::quit();
	}
}
