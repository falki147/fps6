#pragma once

#include <IMG.hpp>
#include <Audio/Sound.hpp>

class SoundCollection {
public:
	enum SoundID {
		SoundShot,
		SoundExplosion,
		SoundOw,
		SoundQuake,
		SoundHardstyle,
		SoundCount
	};

	SoundCollection(IMG& img);

	Sound& operator[](SoundID id) {
		return m_sounds[id];
	}

private:
	Sound m_sounds[SoundCount];
};
