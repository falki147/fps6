#pragma once

#include <IMG.hpp>
#include <Graphics/MD2.hpp>

class MD2Collection {
public:
	enum MD2ID {
		MD2Body,
		MD2CacoEye,
		MD2CacoSkirt,
		MD2CacoMouth,
		MD2Barrel,
		MD2Count
	};

	MD2Collection(IMG& img);

	MD2& operator[](MD2ID id) {
		return m_md2[id];
	}

private:
	MD2 m_md2[MD2Count];
};
