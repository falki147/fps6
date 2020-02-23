#pragma once

#include <istream>
#include <Graphics/VAO.hpp>
#include <Graphics/VBO.hpp>

class MODB {
public:
	MODB();
	MODB(std::istream& stream);

	void load(std::istream& stream);
	void render();

private:
	unsigned int m_numVertices = 0;
	VAO m_vao;
	VBO m_vbo;
};
