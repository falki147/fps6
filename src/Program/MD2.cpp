#include <Program/MD2.hpp>

#ifdef GL_ES_VERSION_3_0
static const char shdVert[] = {
	"#version 300 es\n"

	"in vec3 inPosition0;"
	"in vec2 inTexCoord0;"
	"in vec3 inPosition1;"
	"in vec2 inTexCoord1;"
	"out vec2 vTexCoord;"

	"uniform mat4 uMVP;"
	"uniform float uWeight;"

	"void main() {"
		"gl_Position = uMVP * vec4(mix(inPosition0, inPosition1, uWeight), 1.0);"
		"vTexCoord = mix(inTexCoord0, inTexCoord1, uWeight);"
	"}"
};

static const char shdFrag[] = {
	"#version 300 es\n"
	"precision mediump float;\n"

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
#else
static const char shdVert[] = {
	"#version 130\n"

	"in vec3 inPosition0;"
	"in vec2 inTexCoord0;"
	"in vec3 inPosition1;"
	"in vec2 inTexCoord1;"
	"out vec2 vTexCoord;"

	"uniform mat4 uMVP;"
	"uniform float uWeight;"

	"void main() {"
		"gl_Position = uMVP * vec4(mix(inPosition0, inPosition1, uWeight), 1.0);"
		"vTexCoord = mix(inTexCoord0, inTexCoord1, uWeight);"
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
#endif

MD2Program::MD2Program(): Program(shdVert, shdFrag) {
	bindLocation(0, "inPosition0");
	bindLocation(1, "inColor0");
	bindLocation(2, "inPosition1");
	bindLocation(3, "inColor1");
	link();
	use();

	m_mvp = getUniform("uMVP");
	m_weight = getUniform("uWeight");

	auto loc = getUniform("tex");

	if (loc < 0) {
		use();
		glUniform1i(loc, 0);
	}
}

void MD2Program::setMVP(const mat4& mat) {
	if (m_mvp >= 0)
		glUniformMatrix4fv(m_mvp, 1, GL_TRUE, mat.data);
}

void MD2Program::setWeight(float w) {
	if (m_weight >= 0)
		glUniform1f(m_weight, w);
}
