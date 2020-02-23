#include <Collection/Texture.hpp>

static const char* textureFilenames[TextureCollection::TextureCount] = {
	"wall1.dds",
	"wall2.dds",
	"wall3.dds",
	"wall4.dds",
	"wall5.dds",
	"wall6.dds",
	"floor.dds",
	"ceiling.dds",
	"plant1.dds",
	"plant2.dds",
	"barrel1.dds",
	"barrel2.dds",
	"barrelexp1.dds",
	"barrelexp2.dds",
	"barrelexp3.dds",
	"barrelexp4.dds",
	"barrelexp5.dds",
	"door.dds",
	"monster1.dds",
	"monster2.dds",
	"monster3.dds",
	"monster4.dds",
	"monsterdead1.dds",
	"monsterdead2.dds",
	"monsterdead3.dds",
	"monsterdead4.dds",
	"monsterdead5.dds",
	"monsterdead6.dds",
	"monsterdead7.dds",
	"monsterdead8.dds",
	"caco.dds",
	"cacoeye.dds",
	"cacoskirt.dds",
	"cacomouth.dds",
	"barrel.dds",
	"plant.dds",
	"palm.dds"
};

TextureCollection::TextureCollection(IMG& img) {
	for (auto i = 0u; i < TextureCount; ++i)
		m_textures[i].load(img[textureFilenames[i]]);
}
