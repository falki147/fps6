#include <Entities/Plant1.hpp>
#include <Scenes/Main.hpp>

Plant1::Plant1(vec2 pos): m_position(pos) {
	m_object = Collision::createCircle({ m_position, 8 });
	m_shootable = false;
}

void Plant1::update(MainScene& scene) { }

void Plant1::render(MainScene& scene) {
	if (scene.getCheatStatus()["IWANT3D"]) {
		scene.renderMODB(
			MODBCollection::MODBPalm,
			TextureCollection::TexturePalm,
			mat4::createTranslation(vec3(m_position.x, m_position.y, 0)) * mat4::createScalation(vec3(0.14f, 0.14f, 0.14f))
		);
	}
	else {
		auto& eye = scene.getEye();
		auto dir = (m_position - vec2(eye.x, eye.y)).normalized();

		scene.renderWall(
			TextureCollection::TexturePlant1,
			vec3(m_position.x - 7 * dir.y, m_position.y + 7 * dir.x, 0),
			vec3(m_position.x + 7 * dir.y, m_position.y - 7 * dir.x, 24)
		);
	}
}
