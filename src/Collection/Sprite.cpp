#include <Collection/Sprite.hpp>

static const char* spriteFilenames[] = {
	"gun.dds",
	"chaingun.dds",
	"fork.dds",
	"menu.dds"
};

static SpriteCollection::Info spriteInfo[SpriteCollection::SpriteCount][11] = {
	{
		{ vec2(132, 73), vec2(58, 55),   vec2(0 / 512.f, 193 / 256.f),   vec2(58 / 512.f, 248 / 256.f)  },
		{ vec2(132, 60), vec2(58, 68),   vec2(450 / 512.f, 129 / 256.f), vec2(508 / 512.f, 197 / 256.f) },
		{ vec2(129, 51), vec2(64, 77),   vec2(202 / 512.f, 0 / 256.f),   vec2(266 / 512.f, 77 / 256.f)  },
		{ vec2(132, 73), vec2(58, 55),   vec2(59 / 512.f, 193 / 256.f),  vec2(117 / 512.f, 248 / 256.f) },
		{ vec2(98, 26),  vec2(82, 102),  vec2(209 / 512.f, 129 / 256.f), vec2(291 / 512.f, 231 / 256.f) },
		{ vec2(24, 0),   vec2(120, 128), vec2(0 / 512.f, 0 / 256.f),     vec2(120 / 512.f, 128 / 256.f) },
		{ vec2(116, 49), vec2(80, 79),   vec2(121 / 512.f, 0 / 256.f),   vec2(201 / 512.f, 79 / 256.f)  },
		{ vec2(0, 65),   vec2(198, 63),  vec2(0 / 512.f, 129 / 256.f),   vec2(198 / 512.f, 192 / 256.f) },
		{ vec2(103, 77), vec2(87, 51),   vec2(121 / 512.f, 193 / 256.f), vec2(208 / 512.f, 244 / 256.f) },
		{ vec2(116, 49), vec2(80, 79),   vec2(369 / 512.f, 129 / 256.f), vec2(449 / 512.f, 208 / 256.f) },
		{ vec2(121, 44), vec2(76, 84),   vec2(292 / 512.f, 129 / 256.f), vec2(368 / 512.f, 213 / 256.f) }
	},
	{
		{ vec2(0, 20), vec2(114, 83), vec2(115 / 256.f, 103 / 256.f), vec2(229 / 256.f, 186 / 256.f) },
		{ vec2(0, 1), vec2(114, 102), vec2(115 / 256.f, 0 / 256.f),   vec2(229 / 256.f, 102 / 256.f) },
		{ vec2(0, 0), vec2(114, 103), vec2(0 / 256.f, 0 / 256.f),     vec2(114 / 256.f, 103 / 256.f) }
	},
	{
		{ vec2(0, 30), vec2(73, 104), vec2(70 / 256.f, 135 / 256.f), vec2(143 / 256.f, 239 / 256.f) },
		{ vec2(2, 14), vec2(69, 120), vec2(0 / 256.f, 135 / 256.f),  vec2(69 / 256.f, 255 / 256.f)  },
		{ vec2(0, 0),  vec2(73, 134), vec2(0 / 256.f, 0 / 256.f),    vec2(73 / 256.f, 134 / 256.f)  },
		{ vec2(2, 14), vec2(69, 120), vec2(0 / 256.f, 135 / 256.f),  vec2(69 / 256.f, 255 / 256.f)  }
	},
	{
		{ vec2(0, 0), vec2(300, 100), vec2(0 / 1024.f, 0 / 1024.f),   vec2(300 / 1024.f, 100 / 1024.f) },
		{ vec2(0, 0), vec2(300, 100), vec2(0 / 1024.f, 101 / 1024.f), vec2(300 / 1024.f, 201 / 1024.f) },
		{ vec2(0, 0), vec2(800, 600), vec2(0 / 1024.f, 202 / 1024.f),   vec2(800 / 1024.f, 802 / 1024.f) }
	}
};

SpriteCollection::SpriteCollection(IMG& img) {
	for (auto i = 0u; i < SpriteCount; ++i)
		m_sprites[i].load(img[spriteFilenames[i]]);
}

const SpriteCollection::Info& SpriteCollection::getInfo(SpriteID id, unsigned int sub) const {
	return spriteInfo[id][sub];
}
