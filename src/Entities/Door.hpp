#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>

class Door: public Entity {
public:
	Door(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);
	void hit(class MainScene& scene);

private:
	float m_offsetX = 0;
	vec2 m_position;
};
