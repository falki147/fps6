#include <Graphics/Program.hpp>
#include <vector>
#include <stdexcept>

Program::Program(const std::string& vert, const std::string& frag) {
	GLuint shdVert = 0, shdFrag = 0;

	try {
		shdVert = compileShader(vert, GL_VERTEX_SHADER);
		shdFrag = compileShader(frag, GL_FRAGMENT_SHADER);

		m_prog = glCreateProgram();
		glAttachShader(m_prog, shdVert);
		glAttachShader(m_prog, shdFrag);
		glDeleteShader(shdVert);
		glDeleteShader(shdFrag);
	}
	catch (...) {
		if (shdVert)
			glDeleteShader(shdVert);

		if (shdFrag)
			glDeleteShader(shdFrag);

		throw;
	}
}

Program::~Program() {
	if (m_prog)
		glDeleteProgram(m_prog);
}

void Program::use() const {
	if (m_prog)
		glUseProgram(m_prog);
}

void Program::bindLocation(GLuint index, const std::string& location) {
	if (m_prog)
		glBindAttribLocation(m_prog, index, location.c_str());
}

void Program::link() {
	glLinkProgram(m_prog);
}

GLint Program::getUniform(const std::string & location) {
	return glGetUniformLocation(m_prog, location.c_str());
}

GLuint Program::compileShader(const std::string& src, GLenum type) {
	auto srcPtr = src.c_str();
	GLint srcLen = src.size();
	auto shd = glCreateShader(type);

	if (shd == 0)
		throw std::runtime_error("failed to create shader");

	glShaderSource(shd, 1, &srcPtr, &srcLen);
	glCompileShader(shd);

	GLint param = GL_FALSE;
	glGetShaderiv(shd, GL_COMPILE_STATUS, &param);

	if (param == GL_FALSE) {
		param = 0;
		glGetShaderiv(shd, GL_INFO_LOG_LENGTH, &param);

		std::vector<GLchar> output(param);
		glGetShaderInfoLog(shd, output.size(), nullptr, output.data());
		glDeleteShader(shd);

		throw std::runtime_error(std::string("failed to compile shader: ") + output.data());
	}

	return shd;
}
