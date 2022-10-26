#pragma once

#include <OpenGL.hpp>

class VBO {
public:
	VBO(GLenum usage = GL_STATIC_DRAW);
	VBO(const VBO&) = delete;
	VBO(VBO&& vbo);
	~VBO();

	void data(const void* data, unsigned int length);
	void bind(GLenum target);

	void setUsage(GLenum usage) {
		m_usage = usage;
	}

private:
	GLuint m_vbo = 0;
	GLenum m_usage;
};
