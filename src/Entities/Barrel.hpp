#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>

class Barrel: public Entity {
public:
	Barrel(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);
	void hit(class MainScene& scene);

private:
	float m_index = 0;
	vec2 m_position;
};
