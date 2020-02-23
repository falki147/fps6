#include <Collection/MD2.hpp>

static const char* MD2Filenames[MD2Collection::MD2Count] = {
	"caco.md2",
	"cacoeye.md2",
	"cacoskirt.md2",
	"cacomouth.md2",
	"barrel.md2"
};

MD2Collection::MD2Collection(IMG& img) {
	for (auto i = 0u; i < MD2Count; ++i)
		m_md2[i].load(img[MD2Filenames[i]]);
}
