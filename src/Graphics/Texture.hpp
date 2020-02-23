#pragma once

#include <istream>
#include <glad/glad.h>

class Texture {
public:
	Texture() { }
	Texture(std::istream& stream);
	~Texture();

	void load(std::istream& stream);
	void bindStage(unsigned int ind) const;
	void bind() const;

private:
	GLuint m_texture = 0;
};
