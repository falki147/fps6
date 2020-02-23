#pragma once

#include <Collisions.hpp>

class Entity {
public:
	virtual ~Entity() { }
	virtual void update(class MainScene& scene) = 0;
	virtual void render(class MainScene& scene) = 0;
	virtual void setPosition(vec2 pos) { }
	virtual void hit(class MainScene& scene) { }

	bool hasMoved() const {
		return m_moved;
	}

	bool isShootable() const {
		return m_shootable;
	}

	bool isCollidable() const {
		return m_entityCollision;
	}

	const Collision::Object& getObject() const {
		return m_object;
	}

protected:
	bool m_moved = false;
	bool m_shootable = true;
	bool m_entityCollision = true;
	Collision::Object m_object = Collision::Object();
};
