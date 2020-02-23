#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>

class Plant1: public Entity {
public:
	Plant1(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);

private:
	vec2 m_position;
};
