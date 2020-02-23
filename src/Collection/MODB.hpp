#pragma once

#include <IMG.hpp>
#include <Graphics/MODB.hpp>

class MODBCollection {
public:
	enum MODBID {
		MODBPlant,
		MODBPalm,
		MODBCount
	};

	MODBCollection(IMG& img);

	MODB& operator[](MODBID id) {
		return m_modb[id];
	}

private:
	MODB m_modb[MODBCount];
};
