#include <Collection/Sound.hpp>

static const char* SoundFilenames[SoundCollection::SoundCount] = {
	"shot.wav",
	"explosion.wav",
	"ow.wav",
	"quake.ogg",
	"hardstyle.ogg"
};

SoundCollection::SoundCollection(IMG& img) {
	for (auto i = 0u; i < SoundCount; ++i)
		m_sounds[i] = Sound::fromFile(img[SoundFilenames[i]], img.getSize(SoundFilenames[i]));
}
