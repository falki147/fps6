#include <ResourceHandler.hpp>

ResourceHandler::ResourceHandler(IMG& img):
	m_md2(img), m_modb(img), m_sound(img), m_sprite(img), m_texture(img), m_level(img["level.dat"]) { }
