#include <Math/Matrix.hpp>
#include <Program/Default.hpp>

static const char shdVert[] = {
	"#version 130\n"

	"in vec3 inPosition;"
	"in vec2 inTexCoord;"
	"out vec2 vTexCoord;"

	"uniform mat4 uMVP;"

	"void main() {"
		"gl_Position = uMVP * vec4(inPosition, 1.0);"
		"vTexCoord = inTexCoord;"
	"}"
};

static const char shdFrag[] = {
	"#version 130\n"

	"in vec2 vTexCoord;"
	"out vec4 outColor;"

	"uniform sampler2D tex;"

	"void main() {"
		"vec4 diffuse = texture(tex, vTexCoord);"
		
		"if (diffuse.a < 0.25)"
			"discard;"
		
		"float depth = gl_FragCoord.z / gl_FragCoord.w;"
		"outColor = vec4(diffuse.rgb * (1.0 - depth / 300.0), diffuse.a);"
	"}"
};

DefaultProgram::DefaultProgram(): Program(shdVert, shdFrag) {
	bindLocation(0, "inPosition");
	bindLocation(1, "inTexCoord");
	link();
	use();

	m_mvp = getUniform("uMVP");

	auto loc = getUniform("tex");

	if (loc < 0) {
		use();
		glUniform1i(loc, 0);
	}
}

void DefaultProgram::setMVP(const mat4& mat) {
	if (m_mvp >= 0)
		glUniformMatrix4fv(m_mvp, 1, GL_TRUE, mat.data);
}
