#pragma once

#include <Graphics/Program.hpp>
#include <Math/Matrix.hpp>

class PrimitiveProgram: public Program {
public:
	PrimitiveProgram();
	void setMVP(const mat4& mat);

private:
	GLint m_mvp = 0;
};
