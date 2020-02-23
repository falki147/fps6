#pragma once

#include <Graphics/VAO.hpp>
#include <Graphics/VBO.hpp>
#include <istream>
#include <vector>

class MD2 {
public:
	MD2();
	MD2(std::istream& stream);

	void load(std::istream& stream);
	void render(unsigned int frame);

	unsigned int getNumFrames() {
		return m_vbo.size();
	}

private:
	unsigned int m_numVertices = 0;
	std::vector<VAO> m_vao;
	std::vector<VBO> m_vbo;
};
