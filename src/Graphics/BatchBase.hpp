#pragma once

#include <Graphics/VAO.hpp>
#include <Graphics/VBO.hpp>
#include <vector>

template<typename T> class BatchBase {
public:
	BatchBase(const BatchBase<T>&) = delete;

	void add(const T& vertex) {
		m_data.push_back(vertex);
	}

	void upload() {
		if (m_data.empty())
			m_numVertices = 0;
		else {
			m_numVertices = m_data.size();
			m_vbo.data(m_data.data(), m_data.size() * sizeof(T));
			m_data.clear();
		}
	}

	void render() {
		if (m_numVertices != 0)
			m_vao.draw(GL_TRIANGLES, 0, m_numVertices);
	}

	void beginAttributes() {
		m_vao.begin();
	}

	void endAttributes() {
		m_vao.end();
	}

	void attribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, unsigned int offset) {
		m_vao.attribute(m_vbo, index, size, type, normalized, stride, offset);
	}

	bool canRender() const {
		return m_numVertices != 0;
	}

protected:
	BatchBase(GLenum usage): m_vbo(usage) { }

private:
	std::vector<T> m_data;
	VAO m_vao;
	VBO m_vbo;
	unsigned int m_numVertices = 0;
};
