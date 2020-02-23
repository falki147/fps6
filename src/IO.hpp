#pragma once

#include <istream>
#include <stdexcept>
#include <iostream>
#include <string>

inline void read(std::istream& stream, char* output, unsigned long length) {
	if (!stream.read(output, length)) {
		if (stream.eof())
			throw std::runtime_error("unexpected end of file");

		throw std::runtime_error("failed to read from file");
	}
}

template<typename T> T read(std::istream& stream) {
	T tmp;
	read(stream, (char*) &tmp, sizeof(tmp));
	return tmp;
}

struct SDL_RWops* SDL_RWFromStream(std::istream& stream);
struct SDL_RWops* SDL_RWFromStreamRange(std::istream& stream, unsigned int length);
