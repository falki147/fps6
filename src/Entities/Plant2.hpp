#pragma once

#include <Entities/Entitiy.hpp>
#include <Math/Vector.hpp>

class Plant2: public Entity {
public:
	Plant2(vec2 pos);
	void update(class MainScene& scene);
	void render(class MainScene& scene);

private:
	vec2 m_position;
};
