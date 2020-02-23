#pragma once

#include <Graphics/Program.hpp>
#include <Math/Matrix.hpp>

class MD2Program: public Program {
public:
	MD2Program();
	void setMVP(const mat4& mat);
	void setWeight(float w);

private:
	GLint m_mvp = 0;
	GLint m_weight = 0;
};
