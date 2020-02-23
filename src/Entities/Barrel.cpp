#include "Barrel.hpp"

#include <Scenes/Main.hpp>
#include <Entities/Player.hpp>
#include <Game.hpp>

Barrel::Barrel(vec2 pos): m_position(pos) {
	m_object = Collision::createCircle({ m_position, 8 });
}

void Barrel::update(MainScene& scene) {
	if (m_index < 2) {
		m_index += 0.075f;

		if (m_index > 2)
			m_index = 0;
	}
	else if (m_index < 7) {
		m_index += 0.15f;

		if (m_index >= 7) {
			m_object = Collision::Object();
		}
	}
}

void Barrel::render(MainScene& scene) {
	if (m_index > 7)
		return;
	
	if (scene.getCheatStatus()["IWANT3D"]) {
		scene.renderMD2(
			MD2Collection::MD2Barrel,
			TextureCollection::TextureBarrel,
			m_index * 2,
			mat4::createTranslation(vec3(m_position.x, m_position.y, 0)) * mat4::createScalation(vec3(12.f / 30.f, 12.f / 30.f, 12.f / 30.f))
		);
	}
	else {
		auto& eye = scene.getEye();
		auto dir = (m_position - vec2(eye.x, eye.y)).normalized();

		if (m_index < 2) {
			scene.renderWall(
				(TextureCollection::TextureID) (TextureCollection::TextureBarrel1 + (unsigned int) m_index),
				vec3(m_position.x - 6 * dir.y, m_position.y + 6 * dir.x, 0),
				vec3(m_position.x + 6 * dir.y, m_position.y - 6 * dir.x, 12)
			);
		}
		else {
			scene.renderWall(
				(TextureCollection::TextureID) (TextureCollection::TextureBarrelExp1 + (unsigned int) m_index - 2),
				vec3(m_position.x - 12 * dir.y, m_position.y + 12 * dir.x, 0),
				vec3(m_position.x + 12 * dir.y, m_position.y - 12 * dir.x, 24)
			);
		}
	}
}

void Barrel::hit(class MainScene& scene) {
	const auto damageRadius = 8.f;

	if (m_index < 2) {
		m_index = 2;

		auto player = scene.getPlayer();
		auto dist = (player->getPosition() - m_position).length();

		if (dist < damageRadius + 16)
			player->setHealth(player->getHealth() - (1 - (dist - 16) / damageRadius) * 100);

		Game::getResourceHandler()->getSound()[SoundCollection::SoundExplosion].play();
	}
}
