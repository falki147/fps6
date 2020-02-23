#pragma once

#include <Math/Vector.hpp>
#include <IMG.hpp>
#include <Graphics/Texture.hpp>

class SpriteCollection {
public:
	enum SpriteID {
		SpriteGun,
		SpriteChaingun,
		SpriteFork,
		SpriteMenu,
		SpriteCount
	};

	struct Info {
		vec2 offset;
		vec2 size;
		vec2 p1;
		vec2 p2;
	};

	SpriteCollection(IMG& img);

	const Info& getInfo(SpriteID id, unsigned int sub) const;

	Texture& operator[](SpriteID id) {
		return m_sprites[id];
	}

private:
	Texture m_sprites[SpriteCount];
};
