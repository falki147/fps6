#include "Texture.hpp"

#include <IO.hpp>
#include <vector>
#include <assert.h>

Texture::Texture(std::istream& stream) {
	load(stream);
}

Texture::~Texture() {
	if (m_texture)
		glDeleteTextures(1, &m_texture);
}

void Texture::load(std::istream& stream) {
	if (m_texture)
		glDeleteTextures(1, &m_texture);

	if (read<unsigned int>(stream) != 0x20534444) // "DDS " - magic number
		throw std::runtime_error("invalid DDS file");

	// Read DDS header from file (only relevant fields)
	stream.seekg(8, std::ios::cur);
	auto height = read<unsigned int>(stream);
	auto width = read<unsigned int>(stream);
	stream.seekg(8, std::ios::cur);
	auto mipmaps = read<unsigned int>(stream);
	stream.seekg(52, std::ios::cur);
	auto fourCC = read<unsigned int>(stream);
	auto bitCount = read<unsigned int>(stream);
	auto bitMaskR = read<unsigned int>(stream);
	auto bitMaskG = read<unsigned int>(stream);
	auto bitMaskB = read<unsigned int>(stream);
	auto bitMaskA = read<unsigned int>(stream);
	stream.seekg(20, std::ios::cur);

	GLenum format;
	auto factor = 4ul;
	auto blockSize = 16ul;

	switch (fourCC) {
	case 0x31545844: // "DXT1"
		format = 0x83F1; // COMPRESSED_RGBA_S3TC_DXT1_EXT
		factor = 2;
		blockSize = 8;
		break;
	case 0x33545844: // "DXT3"
		format = 0x83F2; // COMPRESSED_RGBA_S3TC_DXT3_EXT
		break;
	case 0x35545844: // "DXT5"
		format = 0x83F3; // COMPRESSED_RGBA_S3TC_DXT5_EXT
		break;
	default:
		// Make sure images have RGBA format an a bit depth of either 16 or 32
		if ((bitMaskA != 0xF000 || bitMaskR != 0x0F00 || bitMaskG != 0x00F0 || bitMaskB != 0x000F || bitCount != 16) &&
			(bitMaskA != 0xFF000000 || bitMaskR != 0x00FF0000 || bitMaskG != 0x0000FF00 || bitMaskB != 0x000000FF || bitCount != 32))
			throw std::runtime_error("invalid format");

		format = GL_RGBA;
	}

	try {
		// Prepare texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		// Texture has probably atleast one mipmap
		if (mipmaps < 1)
			mipmaps = 1;

		unsigned int i;

		for (i = 0; i < mipmaps; ++i) {
			// If image is empty or stream is empty, skip mipmap
			if (width == 0 || height == 0 || stream.peek() == EOF)
				break;

			if (format == GL_RGBA) {
				auto size = width * height * bitCount / 8;

				std::vector<char> buffer(size);
				read(stream, buffer.data(), size);

				auto type = bitCount == 16 ? GL_UNSIGNED_SHORT_4_4_4_4_REV : GL_UNSIGNED_INT_8_8_8_8_REV;
				glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_BGRA, type, buffer.data());
			}
			else {
				auto size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

				std::vector<char> buffer(size);
				read(stream, buffer.data(), size);

				glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, size, buffer.data());
			}

			width /= 2;
			height /= 2;
		}

		if (i == 0)
			throw std::runtime_error("no mipmap was loaded");

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i - 1);
		glTexParameterf(GL_TEXTURE_2D, 0x84FE, 8);
	}
	catch (...) {
		if (m_texture)
			glDeleteTextures(1, &m_texture);

		throw;
	}
}

void Texture::bindStage(unsigned int ind) const {
	assert(m_texture != 0);

	glActiveTexture(GL_TEXTURE0 + ind);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::bind() const {
	assert(m_texture != 0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
}
