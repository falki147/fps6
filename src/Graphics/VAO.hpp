#pragma once

#include <OpenGL.hpp>

class VAO {
public:
	VAO();
	VAO(const VAO&) = delete;
	VAO(VAO&& vao);
	~VAO();

	void begin();
	void attribute(class VBO& vbo, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, unsigned int offset);
	void end();
	void draw(GLenum mode, GLint first, GLsizei count);

private:
	GLuint m_vao;
};
