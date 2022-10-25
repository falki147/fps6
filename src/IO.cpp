#include <IO.hpp>
#include <SDL_rwops.h>
#include <algorithm>

SDL_RWops* SDL_RWFromStream(std::istream& stream) {
	auto rw = SDL_AllocRW();

	if (!rw)
		return nullptr;

	rw->size = [](SDL_RWops* rw) -> Sint64 {
		return -1;
	};

	rw->seek = [](SDL_RWops* rw, Sint64 offset, int way) -> Sint64 {
		auto& stream = *(std::istream*) rw->hidden.unknown.data1;

		switch (way) {
		case SEEK_SET:
			stream.seekg(offset, std::ios::beg);
			break;
		case SEEK_CUR:
			stream.seekg(offset, std::ios::cur);
			break;
		case SEEK_END:
			stream.seekg(offset, std::ios::end);
			break;
		default:
			return -1;
		}

		return stream.fail() ? -1 : (Sint64) stream.tellg();
	};

	rw->read = [](SDL_RWops* rw, void* ptr, size_t size, size_t maxnum) -> size_t {
		if (size == 0)
			return -1;

		auto& stream = *(std::istream*) rw->hidden.unknown.data1;
		stream.read((char*) ptr, size * maxnum);

		return (size_t) (stream.bad() ? -1 : stream.gcount() / size);
	};

	rw->write = [](SDL_RWops* rw, const void *ptr, size_t size, size_t num) -> size_t {
		return 0;
	};

	rw->close = [](SDL_RWops* rw) -> int {
		if (rw)
			SDL_FreeRW(rw);

		return 0;
	};

	rw->hidden.unknown.data1 = &stream;

	return rw;
}

SDL_RWops* SDL_RWFromStreamRange(std::istream& stream, unsigned int length) {
	auto rw = SDL_AllocRW();

	if (!rw)
		return nullptr;

	rw->size = [](SDL_RWops* rw) -> Sint64 {
		auto start = (fpos_t) rw->hidden.mem.here;
		auto stop = (fpos_t) rw->hidden.mem.stop;

		return stop - start;
	};

	rw->seek = [](SDL_RWops* rw, Sint64 offset, int way) -> Sint64 {
		auto& stream = *(std::istream*) rw->hidden.mem.base;
		auto start = (fpos_t) rw->hidden.mem.here;
		auto stop = (fpos_t) rw->hidden.mem.stop;

		switch (way) {
		case SEEK_SET:
			stream.seekg(start + offset, std::ios::beg);
			break;
		case SEEK_CUR:
			stream.seekg(offset, std::ios::cur);
			break;
		case SEEK_END:
			stream.seekg(stop + offset, std::ios::beg);
			break;
		default:
			return -1;
		}

		return stream.fail() ? -1 : (Sint64) (stream.tellg() - start);
	};

	rw->read = [](SDL_RWops* rw, void* ptr, size_t size, size_t maxnum) -> size_t {
		if (size == 0)
			return -1;

		auto& stream = *(std::istream*) rw->hidden.mem.base;
		auto stop = (std::streamoff) rw->hidden.mem.stop;

		auto numBytes = std::min((std::streamoff) (size * maxnum), (std::streamoff) (stop - stream.tellg()));
		stream.read((char*) ptr, numBytes);

		return (size_t) (stream.bad() ? -1 : stream.gcount() / size);
	};

	rw->write = [](SDL_RWops* rw, const void *ptr, size_t size, size_t num) -> size_t {
		return 0;
	};

	rw->close = [](SDL_RWops* rw) -> int {
		if (rw)
			SDL_FreeRW(rw);

		return 0;
	};

	rw->hidden.mem.base = (Uint8*) &stream;
	rw->hidden.mem.here = (Uint8*) (std::streamoff) stream.tellg();
	rw->hidden.mem.stop = (Uint8*) ((std::streamoff) stream.tellg() + length);

	return rw;
}
