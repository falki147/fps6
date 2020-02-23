#pragma once

#include <Math/Matrix.hpp>
#include <Graphics/Program.hpp>

class DefaultProgram: public Program {
public:
	DefaultProgram();
	void setMVP(const mat4& mat);

private:
	GLint m_mvp = 0;
};
