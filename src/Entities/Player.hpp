#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>
#include <Audio/Sound.hpp>

class Player: public Entity {
public:
	Player(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);
	void setPosition(vec2 pos);
	void setHealth(float health);

	const vec2& getPosition() const {
		return m_position;
	}

	float getHealth() const {
		return m_health;
	}
	
private:
	float m_z = 16;
	float m_health = 100;
	float m_index = 0;
	float m_direction = 0;
	bool m_shootingAnimation = false;
	vec2 m_position;
};
