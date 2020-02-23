#include "MODB.hpp"
#include <IO.hpp>
#include <vector>

MODB::MODB() { }

MODB::MODB(std::istream& stream) {
	load(stream);
}

void MODB::load(std::istream& stream) {
	enum {
		FlagNormals   = 1,
		FlagTexCoords = 2,
		FlagColors    = 4
	};

	if (read<unsigned int>(stream) != 0x42444F4D)
		throw std::runtime_error("invalid modle file");

	m_numVertices = read<unsigned int>(stream);
	auto flags = read<unsigned int>(stream);
	stream.seekg(8, std::ios::cur); // skip ambient & specular

	std::vector<float> data;
	data.reserve(m_numVertices * 5);

	for (auto i = 0u; i < m_numVertices; ++i) {
		data.push_back(read<float>(stream));
		data.push_back(read<float>(stream));
		data.push_back(read<float>(stream));

		if (flags & FlagNormals)
			stream.seekg(12, std::ios::cur);

		if (flags & FlagTexCoords) {
			data.push_back(read<float>(stream));
			data.push_back(read<float>(stream));
		}
		else {
			data.push_back(0);
			data.push_back(0);
		}

		if (flags & FlagColors)
			stream.seekg(12, std::ios::cur);
	}

	m_vbo.data(data.data(), data.size() * sizeof(float));
	
	m_vao.begin();
	m_vao.attribute(m_vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	m_vao.attribute(m_vbo, 1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 12);
	m_vao.end();
}

void MODB::render() {
	m_vao.draw(GL_TRIANGLES, 0, m_numVertices);
}
