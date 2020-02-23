#include <Collection/MODB.hpp>

static const char* MODBFilenames[MODBCollection::MODBCount] = {
	"plant.modb",
	"palm.modb"
};

MODBCollection::MODBCollection(IMG& img) {
	for (auto i = 0u; i < MODBCount; ++i)
		m_modb[i].load(img[MODBFilenames[i]]);
}
