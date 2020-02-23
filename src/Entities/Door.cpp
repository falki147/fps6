#include "Door.hpp"

#include <Scenes/Main.hpp>

Door::Door(vec2 pos): m_position(pos) {
	m_object = Collision::createLine({ vec2(pos.x - 16, pos.y), vec2(pos.x + 16, pos.y)});
}

void Door::update(MainScene& scene) {
	if (m_offsetX != 0) {
		m_offsetX -= 0.75f;

		if (m_offsetX < -32)
			m_offsetX = -32;

		m_object = Collision::createLine({
			vec2(m_position.x + m_offsetX - 16, m_position.y),
			vec2(m_position.x + m_offsetX + 16, m_position.y)
		});
	}
}

void Door::render(MainScene& scene) {
	scene.renderWall(
		TextureCollection::TextureDoor,
		vec3(m_position.x + m_offsetX - 16, m_position.y, 0),
		vec3(m_position.x + m_offsetX + 16, m_position.y, 32)
	);
}

void Door::hit(class MainScene& scene) {
	if (m_offsetX == 0)
		m_offsetX = -0.75f;
}
