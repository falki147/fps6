#include "MD2.hpp"

#include <array>
#include <IO.hpp>
#include <Math/Vector.hpp>

MD2::MD2() { }

MD2::MD2(std::istream& stream) {
	load(stream);
}

void MD2::load(std::istream& stream) {
	m_vao.clear();
	m_vbo.clear();

	struct TextureCoord {
		unsigned short s, t;
	};

	struct Triangle {
		unsigned short vertex[3];
		unsigned short st[3];
	};

	struct CompressedVertex {
		unsigned char x, y, z;
		unsigned char normal;
	};

	struct Frame {
		vec3 scale;
		vec3 trans;
		std::array<char, 16> name;
	};

	struct Vertex {
		vec3 position;
		vec2 texcoord;
	};

	auto base = stream.tellg();
	
	if (read<unsigned int>(stream) != 0x32504449)
		throw std::runtime_error("invalid file");

	if (read<unsigned int>(stream) != 8)
		throw std::runtime_error("invalid file");

	auto width = (float) read<unsigned int>(stream);
	auto height = (float) read<unsigned int>(stream);
	stream.seekg(8, std::ios::cur);
	auto numVertices = read<unsigned int>(stream);
	auto numTextureCoords = read<unsigned int>(stream);
	auto numTriangles = read<unsigned int>(stream);
	stream.seekg(4, std::ios::cur);
	auto numFrames = read<unsigned int>(stream);
	stream.seekg(4, std::ios::cur);
	auto offsetTextureCoords = read<unsigned int>(stream);
	auto offsetTris = read<unsigned int>(stream);
	auto offsetFrames = read<unsigned int>(stream);

	stream.seekg(offsetTextureCoords + base);
	std::vector<TextureCoord> textureCoords(numTextureCoords);
	read(stream, (char*) textureCoords.data(), textureCoords.size() * sizeof(TextureCoord));

	stream.seekg(offsetTris + base);
	std::vector<Triangle> triangles(numTriangles);
	read(stream, (char*) triangles.data(), triangles.size() * sizeof(Triangle));

	m_vbo.reserve(numFrames);

	stream.seekg(offsetFrames + base);

	std::vector<bool> isFirstFrame(numFrames);
	std::array<char, 16> lastName;

	for (auto i = 0u; i < numFrames; ++i) {
		auto frame = read<Frame>(stream);

		if (i == 0 || strncmp(lastName.data(), frame.name.data(), 16) != 0) {
			lastName = frame.name;
			isFirstFrame[i] = true;
		}

		std::vector<CompressedVertex> compressedVertices(numVertices);
		read(stream, (char*) compressedVertices.data(), compressedVertices.size() * sizeof(CompressedVertex));

		std::vector<Vertex> vertices;
		vertices.reserve(numVertices);

		for (auto i = 0u; i < numTriangles; ++i) {
			for (auto j = 0u; j < 3; ++j) {
				vertices.push_back({
					vec3(
						compressedVertices[triangles[i].vertex[j]].x * frame.scale.x,
						compressedVertices[triangles[i].vertex[j]].y * frame.scale.y,
						compressedVertices[triangles[i].vertex[j]].z * frame.scale.z
					) + frame.trans,
					vec2(
						textureCoords[triangles[i].st[j]].s / width,
						textureCoords[triangles[i].st[j]].t / height
					)
				});
			}
		}

		m_vbo.emplace_back();
		m_vbo.back().data(vertices.data(), vertices.size() * sizeof(Vertex));
	}

	m_vao.reserve(numFrames);
	auto first = 0u;

	for (auto i = 0u; i < numFrames; ++i) {
		if (isFirstFrame[i])
			first = i;

		auto nextFrame = (i + 1) % numFrames;
		auto next = isFirstFrame[nextFrame] ? first : nextFrame;

		m_vao.emplace_back();
		m_vao.back().begin();
		m_vao.back().attribute(m_vbo[i], 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		m_vao.back().attribute(m_vbo[i], 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 12);
		m_vao.back().attribute(m_vbo[next], 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		m_vao.back().attribute(m_vbo[next], 3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 12);
		m_vao.back().end();
	}

	m_numVertices = numTriangles * 3;
}

void MD2::render(unsigned int frame) {
	if (frame >= getNumFrames())
		return;

	m_vao[frame].draw(GL_TRIANGLES, 0, m_numVertices);
}
