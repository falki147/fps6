#pragma once

#include <Math/Vector.hpp>

namespace Collision {
	struct Circle {
		vec2 pos;
		float rad;
	};

	struct Line {
		vec2 p1;
		vec2 p2;
	};

	struct Ray {
		vec2 origin;
		vec2 dir;
	};

	struct Object {
		enum Type {
			TypeNone,
			TypeCircle,
			TypeLine
		};

		Object() {
			type = TypeNone;
		}

		Type type;

		union {
			Circle circle;
			Line line;
		};
	};

	inline Object createCircle(Circle l) {
		Object o;

		o.type = Object::TypeCircle;
		o.circle = l;

		return o;
	}

	inline Object createLine(Line l) {
		Object o;

		o.type = Object::TypeLine;
		o.line = l;

		return o;
	}

	bool collides(const Circle& c1, const Circle& c2);
	bool collides(const Circle& c, const Line& l);
	bool collides(const Circle& c, const Object& o);
	bool collides(const Line& l, const Circle& c);
	bool collides(const Line& l1, const Line& l2);
	bool collides(const Line& l, const Object& o);
	float cast(const Ray& r, const Circle& c);
	float cast(const Ray& r, const Line& l);
	float cast(const Ray& r, const Object& o);
	void resolve(Circle& c1, const Circle& c2);
	void resolve(Circle& c, const Line& l);
	void resolve(Circle& c, const Object& o);
}
