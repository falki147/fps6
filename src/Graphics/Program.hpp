#pragma once

#include <string>
#include <glad/glad.h>

class Program {
public:
	Program(const std::string& vert, const std::string& frag);
	Program(const Program&) = delete;
	~Program();

	void use() const;
	void bindLocation(GLuint index, const std::string& location);
	void link();
	GLint getUniform(const std::string& location);

private:
	GLuint compileShader(const std::string& src, GLenum type);

	GLuint m_prog = 0;
};
