#include <Graphics/VBO.hpp>

VBO::VBO(GLenum usage): m_usage(usage) {
	glGenBuffers(1, &m_vbo);
}

VBO::VBO(VBO&& vbo) {
	m_usage = vbo.m_usage;
	m_vbo = vbo.m_vbo;

	vbo.m_vbo = 0;
	glGenBuffers(1, &vbo.m_vbo);
}

VBO::~VBO() {
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
}

void VBO::data(const void* data, unsigned int length) {
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, length, data, m_usage);
}

void VBO::bind(GLenum target) {
	glBindBuffer(target, m_vbo);
}
