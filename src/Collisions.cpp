#include <Collisions.hpp>

bool Collision::collides(const Circle& c1, const Circle& c2) {
	return (c1.pos - c2.pos).lengthSquared() < (c1.rad + c2.rad) * (c1.rad + c2.rad);
}

bool Collision::collides(const Circle& c, const Line& l) {
	auto delta = l.p2 - l.p1;
	vec2 p;

	if (delta.x == 0 && delta.y == 0)
		p = l.p1;
	else {
		auto t = (delta * (c.pos - l.p1)) / delta.lengthSquared();
		t = std::min(std::max(t, 0.0f), 1.0f);
		p = l.p1 + delta * t;
	}

	return (c.pos - p).lengthSquared() < c.rad * c.rad;
}

bool Collision::collides(const Circle& c, const Object& o) {
	switch (o.type) {
	case Object::TypeCircle:
		return collides(c, o.circle);
	case Object::TypeLine:
		return collides(c, o.line);
	}

	return false;
}

bool Collision::collides(const Line& l, const Circle& c) {
	auto delta = l.p2 - l.p1;
	vec2 p;

	if (delta.x == 0 && delta.y == 0)
		p = l.p1;
	else {
		auto t = (delta * (c.pos - l.p1)) / delta.lengthSquared();
		t = std::min(std::max(t, 0.0f), 1.0f);
		p = l.p1 + delta * t;
	}

	return (p - c.pos).lengthSquared() < c.rad * c.rad;
}

bool Collision::collides(const Line& l1, const Line& l2) {
	auto r = l1.p2 - l1.p1;
	auto s = l2.p2 - l2.p1;

	auto rs = r.x * s.y - r.y * s.x;
	
	if (rs == 0)
		return false;

	auto d = l2.p1 - l1.p1;

	auto t = (d.x * s.y - d.y * s.x) / rs;
	auto u = (d.x * r.y - d.y * r.x) / rs;

	return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}

bool Collision::collides(const Line& l, const Object& o) {
	switch (o.type) {
	case Object::TypeCircle:
		return collides(l, o.circle);
	case Object::TypeLine:
		return collides(l, o.line);
	}

	return false;
}

float Collision::cast(const Ray& r, const Circle& c) {
	auto h = c.pos - r.origin;
	auto lf = r.dir.normalized() * h;
	auto s = std::sqrt(c.rad * c.rad - h * h + lf * lf);

	auto l1 = lf + s;
	auto l2 = lf - s;

	if (l1 < 0 && l2 < 0)
		return -1;

	if (l1 < 0 || l2 < 0)
		return std::max(l1, l2);

	return std::min(l1, l2);
}

float Collision::cast(const Ray& r, const Line& l) {
	auto v1 = r.origin - l.p1;
	auto v2 = l.p2 - l.p1;
	auto v3 = vec2(-r.dir.y, r.dir.x);

	auto t = (v1 * v3) / (v2 * v3);

	if (t < 0 || t > 1)
		return -1;

	auto p = l.p1 + v2 * t - r.origin;

	if (p * r.dir < 0)
		return -1;

	return p.length();
}

float Collision::cast(const Ray& r, const Object& o) {
	switch (o.type) {
	case Object::TypeCircle:
		return cast(r, o.circle);
	case Object::TypeLine:
		return cast(r, o.line);
	}

	return -1;
}

void Collision::resolve(Circle& c1, const Circle& c2) {
	auto dir = (c1.pos - c2.pos).normalized();
	c1.pos = c2.pos + dir * (c1.rad + c2.rad);
}

void Collision::resolve(Circle& c, const Line& l) {
	auto delta = l.p2 - l.p1;
	vec2 p;

	if (delta.x == 0 && delta.y == 0)
		p = l.p1;
	else {
		auto t = (delta * (c.pos - l.p1)) / delta.lengthSquared();
		t = std::min(std::max(t, 0.0f), 1.0f);
		p = l.p1 + delta * t;
	}

	auto dir = (c.pos - p).normalized();
	c.pos = p + dir * c.rad;
}

void Collision::resolve(Circle& c, const Object& o) {
	switch (o.type) {
	case Object::TypeCircle:
		resolve(c, o.circle);
		break;
	case Object::TypeLine:
		resolve(c, o.line);
		break;
	}
}
