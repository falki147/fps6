#pragma once

#include <Math/Matrix.hpp>
#include <Graphics/Program.hpp>

class SpriteProgram: public Program {
public:
	SpriteProgram();
	void setMVP(const mat4& mat);

private:
	GLint m_mvp = 0;
};
