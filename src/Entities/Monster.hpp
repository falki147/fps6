#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>

class Monster: public Entity {
public:
	Monster(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);
	void hit(class MainScene& scene);
	void setPosition(vec2 pos);

private:
	float m_index;
	vec2 m_position;
};
