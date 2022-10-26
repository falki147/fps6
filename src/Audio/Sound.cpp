#include <Audio/Sound.hpp>
#include <IO.hpp>

#ifdef FPS6_NO_AUDIO

// Define as empty struct to keep compiler happy
struct Mix_Chunk {};

Sound::Sound(struct Mix_Chunk* chunk): m_chunk(chunk) { }

Sound::~Sound() { }

void Sound::play(bool loop) { }

bool Sound::isPlaying() { return false; }

void Sound::stop() { }

Sound Sound::fromFile(const std::string& file) {
	return Sound(nullptr);
}

Sound Sound::fromFile(std::istream& stream) {
	return Sound(nullptr);
}

Sound Sound::fromFile(std::istream& stream, unsigned int length) {
	return Sound(nullptr);
}

#else

#include <SDL_mixer.h>

static Mix_Chunk* g_channels[MIX_CHANNELS] = { 0 };

Sound::Sound(struct Mix_Chunk* chunk): m_chunk(chunk) {
	if (chunk == nullptr)
		throw std::runtime_error(Mix_GetError());//"mix chunk is null");
}

Sound::~Sound() {
	if (m_chunk)
		Mix_FreeChunk(m_chunk);
}

void Sound::play(bool loop) {
	auto i = 0u;

	while (i < MIX_CHANNELS && g_channels[i] != nullptr && Mix_Playing(i) != 0)
		++i;

	if (i >= MIX_CHANNELS)
		return;

	g_channels[i] = m_chunk;
	Mix_PlayChannel(i, m_chunk, loop ? -1 : 0);
}

bool Sound::isPlaying() {
	auto i = 0u;

	while (i < MIX_CHANNELS && (g_channels[i] != m_chunk || Mix_Playing(i) == 0))
		++i;

	return i < MIX_CHANNELS;
}

void Sound::stop() {
	auto i = 0u;

	while (i < MIX_CHANNELS && (g_channels[i] != m_chunk || Mix_Playing(i) == 0))
		++i;

	if (i >= MIX_CHANNELS)
		return;

	g_channels[i] = nullptr;
	Mix_HaltChannel(i);
}

Sound Sound::fromFile(const std::string& file) {
	return Sound(Mix_LoadWAV(file.c_str()));
}

Sound Sound::fromFile(std::istream& stream) {
	return Sound(Mix_LoadWAV_RW(SDL_RWFromStream(stream), 1));
}

Sound Sound::fromFile(std::istream& stream, unsigned int length) {
	return Sound(Mix_LoadWAV_RW(SDL_RWFromStreamRange(stream, length), 1));
}

#endif
