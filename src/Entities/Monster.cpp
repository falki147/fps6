#include "Monster.hpp"
#include <Scenes/Main.hpp>
#include <Entities/Player.hpp>
#include <Game.hpp>

Monster::Monster(vec2 pos): m_position(pos), m_index((float) (std::rand() % 4)) {
	m_object = Collision::createCircle({ m_position, 8 });
	m_entityCollision = false;
}

void Monster::update(MainScene& scene) {
	const auto normalFrames = 4.f;
	const auto deadFrames = 7.f;

	if (m_index < normalFrames) {
		m_index += 0.225f;

		if (m_index > normalFrames)
			m_index = 0;

		auto player = scene.getPlayer();
		
		m_moved = false;
		auto len = (player->getPosition() - m_position).lengthSquared();

		if (len < 200 * 200) {
			auto collides = false;
			Collision::Line lineOfSight = { m_position, player->getPosition() };

			for (auto& line : scene.getCollisionLines()) {
				if (Collision::collides(lineOfSight, line)) {
					collides = true;
					break;
				}
			}

			if (!collides) {
				for (auto& entity : scene.getEntities()) {
					if (this != entity && entity->isCollidable() && Collision::collides(lineOfSight, entity->getObject())) {
						collides = true;
						break;
					}
				}
			}

			if (!collides) {
				if (len > 12 * 12) {
					auto dir = (player->getPosition() - m_position).normalized();
					m_position = m_position + dir * 1.05f;
					m_object.circle = { m_position, 8 };
					m_moved = true;
				}
				else {
					auto& sound = Game::getResourceHandler()->getSound()[SoundCollection::SoundOw];

					if (!sound.isPlaying())
						sound.play();

					player->setHealth(player->getHealth() - 1.5f);
				}
			}
		}
	}
	else if (m_index < normalFrames + deadFrames)
		m_index += 0.15f;
}

void Monster::render(MainScene& scene) {
	auto& eye = scene.getEye();
	auto dir = (m_position - vec2(eye.x, eye.y)).normalized();

	if (scene.getCheatStatus()["IWANT3D"]) {
		auto mat = mat4::createTranslation(vec3(m_position.x, m_position.y, 8.5f)) * mat4::createRotationZ(-dir.y, -dir.x) * mat4::createScalation(vec3(0.11f, 0.11f, 0.11f));
		auto ind = m_index < 4 ? m_index * 1.5f : (m_index - 4) * 4 / 7 + 6;

		scene.renderMD2(MD2Collection::MD2Body, TextureCollection::TextureCaco, ind, mat);
		scene.renderMD2(MD2Collection::MD2CacoEye, TextureCollection::TextureCacoEye, ind, mat);
		scene.renderMD2(MD2Collection::MD2CacoMouth, TextureCollection::TextureCacoMouth, ind, mat);
		scene.renderMD2(MD2Collection::MD2CacoSkirt, TextureCollection::TextureCacoSkirt, ind, mat);
	}
	else {
		if (m_index < 4) {
			scene.renderWall(
				(TextureCollection::TextureID) (TextureCollection::TextureMonster1 + (unsigned int) m_index),
				vec3(m_position.x - 8 * dir.y, m_position.y + 8 * dir.x, 3),
				vec3(m_position.x + 8 * dir.y, m_position.y - 8 * dir.x, 19)
			);
		}
		else {
			scene.renderWall(
				(TextureCollection::TextureID) (TextureCollection::TextureMonsterDead1 + (unsigned int) m_index - 4),
				vec3(m_position.x - 8 * dir.y, m_position.y + 8 * dir.x, -13),
				vec3(m_position.x + 8 * dir.y, m_position.y - 8 * dir.x, 19)
			);
		}
	}
}

void Monster::hit(class MainScene& scene) {
	const auto normalFrames = 4.f;

	if (m_index < normalFrames) {
		m_index = normalFrames;
		m_object = Collision::Object();
		scene.removeMonster();
	}
}

void Monster::setPosition(vec2 pos) {
	m_position = pos;
	m_object.circle = { m_position, 8 };
}
