#include <Program/Primitive.hpp>

#ifdef GL_ES_VERSION_3_0
static const char shdVert[] = {
	"#version 300 es\n"

	"in vec3 inPosition;"
	"in vec4 inColor;"
	"out vec4 vColor;"

	"uniform mat4 uMVP;"

	"void main() {"
		"gl_Position = uMVP * vec4(inPosition, 1.0);"
		"vColor = inColor;"
	"}"
};

static const char shdFrag[] = {
	"#version 300 es\n"
	"precision mediump float;\n"

	"in vec4 vColor;"
	"out vec4 outColor;"

	"uniform sampler2D tex;"

	"void main() {"
		"outColor = vColor;"
	"}"
};
#else
static const char shdVert[] = {
	"#version 130\n"

	"in vec3 inPosition;"
	"in vec4 inColor;"
	"out vec4 vColor;"

	"uniform mat4 uMVP;"

	"void main() {"
		"gl_Position = uMVP * vec4(inPosition, 1.0);"
		"vColor = inColor;"
	"}"
};

static const char shdFrag[] = {
	"#version 130\n"

	"in vec4 vColor;"
	"out vec4 outColor;"

	"uniform sampler2D tex;"

	"void main() {"
		"outColor = vColor;"
	"}"
};
#endif

PrimitiveProgram::PrimitiveProgram(): Program(shdVert, shdFrag) {
	bindLocation(0, "inPosition");
	bindLocation(1, "inColor");
	link();
	m_mvp = getUniform("uMVP");
}

void PrimitiveProgram::setMVP(const mat4& mat) {
	if (m_mvp >= 0)
		glUniformMatrix4fv(m_mvp, 1, GL_TRUE, mat.data);
}
