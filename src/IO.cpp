#include <IO.hpp>
#include <SDL_rwops.h>
#include <algorithm>

struct InternalData {
	std::istream* stream{};
	Sint64 start{}, stop{};
};

SDL_RWops* SDL_RWFromStream(std::istream& stream) {
	auto rw = SDL_AllocRW();

	if (!rw)
		return nullptr;

	rw->type = 0;

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

	rw->type = 0;

	const auto internalData = new InternalData();
	internalData->stream = &stream;
	internalData->start = stream.tellg();
	internalData->stop = (Sint64) stream.tellg() + length;
	rw->hidden.unknown.data1 = reinterpret_cast<void*>(internalData);

	rw->size = [](SDL_RWops* rw) -> Sint64 {
		const auto internalData = reinterpret_cast<InternalData*>(rw->hidden.unknown.data1);
		return internalData->stop - internalData->start;
	};

	rw->seek = [](SDL_RWops* rw, Sint64 offset, int way) -> Sint64 {
		const auto internalData = reinterpret_cast<InternalData*>(rw->hidden.unknown.data1);

		auto& stream = *internalData->stream;
		auto start = internalData->start;
		auto stop = internalData->stop;

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

		const auto internalData = reinterpret_cast<InternalData*>(rw->hidden.unknown.data1);

		auto& stream = *internalData->stream;
		auto stop = internalData->stop;

		auto numBytes = std::min((std::streamoff) (size * maxnum), (std::streamoff) (stop - stream.tellg()));
		stream.read((char*) ptr, numBytes);

		return (size_t) (stream.bad() ? -1 : stream.gcount() / size);
	};

	rw->write = [](SDL_RWops* rw, const void *ptr, size_t size, size_t num) -> size_t {
		return 0;
	};

	rw->close = [](SDL_RWops* rw) -> int {
		if (rw) {
			const auto internalData = reinterpret_cast<InternalData*>(rw->hidden.unknown.data1);
			delete internalData;

			SDL_FreeRW(rw);
		}

		return 0;
	};

	return rw;
}
