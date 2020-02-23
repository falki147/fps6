#pragma once

#include <IMG.hpp>
#include <Collection/MD2.hpp>
#include <Collection/MODB.hpp>
#include <Collection/Sound.hpp>
#include <Collection/Sprite.hpp>
#include <Collection/Texture.hpp>
#include <Level.hpp>

class ResourceHandler {
public:
	ResourceHandler(IMG& img);

	auto& getMD2() {
		return m_md2;
	}

	auto& getMODB() {
		return m_modb;
	}

	auto& getSound() {
		return m_sound;
	}

	auto& getSprite() {
		return m_sprite;
	}

	auto& getTexture() {
		return m_texture;
	}

	auto& getLevel() {
		return m_level;
	}

private:
	MD2Collection m_md2;
	MODBCollection m_modb;
	SoundCollection m_sound;
	SpriteCollection m_sprite;
	TextureCollection m_texture;
	Level m_level;
};
