#pragma once

#include <istream>
#include <vector>
#include <Math/Vector.hpp>
#include <Dungeon.hpp>

class Level {
public:
	struct Entity {
		unsigned short id;
		vec2 pos;
	};

	enum ID {
		IDPlayer,
		IDWall1Hor,
		IDWall2Hor,
		IDWall3Hor,
		IDWall4Hor,
		IDWall5Hor,
		IDWall6Hor,
		IDWall1Vert,
		IDWall2Vert,
		IDWall3Vert,
		IDWall4Vert,
		IDWall5Vert,
		IDWall6Vert,
		IDDoor,
		IDBarrel,
		IDMonster1,
		IDPlant1,
		IDPlant2
	};

	Level(std::istream& stream);
	Level(Dungeon::Grid& grid);

	const std::vector<Entity> getEntities() const {
		return m_entities;
	}

private:
	void placeWalls(Dungeon::Grid& grid, unsigned int x, unsigned int y, ID hor, ID vert);
	void placePlants(Dungeon::Grid& grid, unsigned int x, unsigned int y);
	void placeDoor(Dungeon::Grid& grid, unsigned int x, unsigned int y);

	std::vector<Entity> m_entities;
};
