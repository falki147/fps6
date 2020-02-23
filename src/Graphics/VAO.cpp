#include <Graphics/VAO.hpp>

#include <cstdint>
#include <Graphics/VBO.hpp>

VAO::VAO() {
	glGenVertexArrays(1, &m_vao);
}

VAO::VAO(VAO&& vao) {
	m_vao = vao.m_vao;
	vao.m_vao = 0;
	glGenVertexArrays(1, &vao.m_vao);
}

VAO::~VAO() {
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
}

void VAO::begin() {
	glBindVertexArray(m_vao);
}

void VAO::attribute(VBO& vbo, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, unsigned int offset) {
	vbo.bind(GL_ARRAY_BUFFER);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, (void*) (std::uintptr_t) offset);
}

void VAO::end() {
	glBindVertexArray(0);
}

void VAO::draw(GLenum mode, GLint first, GLsizei count) {
	glBindVertexArray(m_vao);
	glDrawArrays(mode, first, count);
	glBindVertexArray(0);
}
