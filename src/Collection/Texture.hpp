#pragma once

#include <IMG.hpp>
#include <Graphics/Texture.hpp>

class TextureCollection {
public:
	enum TextureID {
		TextureWall1,
		TextureWall2,
		TextureWall3,
		TextureWall4,
		TextureWall5,
		TextureWall6,
		TextureFloor,
		TextureCeiling,
		TexturePlant1,
		TexturePlant2,
		TextureBarrel1,
		TextureBarrel2,
		TextureBarrelExp1,
		TextureBarrelExp2,
		TextureBarrelExp3,
		TextureBarrelExp4,
		TextureBarrelExp5,
		TextureDoor,
		TextureMonster1,
		TextureMonster2,
		TextureMonster3,
		TextureMonster4,
		TextureMonsterDead1,
		TextureMonsterDead2,
		TextureMonsterDead3,
		TextureMonsterDead4,
		TextureMonsterDead5,
		TextureMonsterDead6,
		TextureMonsterDead7,
		TextureMonsterDead8,
		TextureCaco,
		TextureCacoEye,
		TextureCacoSkirt,
		TextureCacoMouth,
		TextureBarrel,
		TexturePlant,
		TexturePalm,
		TextureCount
	};

	TextureCollection(IMG& img);

	Texture& operator[](TextureID id) {
		return m_textures[id];
	}

private:
	Texture m_textures[TextureCount];
};
