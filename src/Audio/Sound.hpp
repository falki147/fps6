#pragma once

#include <string>

class Sound {
public:
	Sound() { }
	Sound(const Sound&) = delete;
	Sound(Sound&& sound): m_chunk(sound.m_chunk) { sound.m_chunk = nullptr; }
	Sound(struct Mix_Chunk* chunk);
	~Sound();

	void play(bool loop = false);
	bool isPlaying();
	void stop();

	void operator=(Sound&& sound) {
		m_chunk = sound.m_chunk;
		sound.m_chunk = nullptr;
	}

	static Sound fromFile(const std::string& file);
	static Sound fromFile(std::istream& stream);
	static Sound fromFile(std::istream& stream, unsigned int length);

private:
	struct Mix_Chunk* m_chunk = nullptr;
};
