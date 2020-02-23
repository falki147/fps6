#include <Level.hpp>
#include <IO.hpp>
#include <map>

Level::Level(std::istream& stream) {
	auto size = read<unsigned int>(stream);
	m_entities.reserve(size);

	for (auto i = 0u; i < size; ++i) {
		Entity e;

		e.id = read<unsigned short>(stream);
		e.pos.x = read<float>(stream);
		e.pos.y = -read<float>(stream);

		m_entities.push_back(e);
	}
}

Level::Level(Dungeon::Grid& grid) {
	std::map<unsigned int, unsigned int> regions;

	for (auto j = 1u; j < grid.height() - 1; ++j) {
		for (auto i = 1u; i < grid.width() - 1; ++i) {
			const auto region = grid(i, j).region;

			unsigned int wall;
			auto it = regions.find(region);

			if (it == regions.end())
				regions[region] = wall = std::rand() % 6;
			else
				wall = it->second;

			switch (grid(i, j).type) {
			case Dungeon::TileStart:
				m_entities.push_back({ IDPlayer, vec2(i * 32, j * 32) });
				break;
			case Dungeon::TileMonster:
				m_entities.push_back({ IDMonster1, vec2(i * 32, j * 32) });
				break;
			case Dungeon::TilePlant:
				placePlants(grid, i, j);
				break;
			case Dungeon::TileDoor:
				placeDoor(grid, i, j);
				break;
			}

			placeWalls(grid, i, j, (ID) (IDWall1Hor + wall), (ID) (IDWall1Vert + wall));
		}
	}
}

void Level::placeWalls(Dungeon::Grid& grid, unsigned int x, unsigned int y, ID hor, ID vert) {
	if (grid(x + 1, y).type == Dungeon::TileWall)
		m_entities.push_back({ (unsigned short) vert, vec2(x * 32 + 16, y * 32) });
				
	if (grid(x - 1, y).type == Dungeon::TileWall)
		m_entities.push_back({ (unsigned short) vert, vec2(x * 32 - 16, y * 32)});

	if (grid(x, y + 1).type == Dungeon::TileWall)
		m_entities.push_back({ (unsigned short) hor, vec2(x * 32, y * 32 + 16) });
				
	if (grid(x, y - 1).type == Dungeon::TileWall)
		m_entities.push_back({ (unsigned short) hor, vec2(x * 32, y * 32 - 16) });
}

void Level::placePlants(Dungeon::Grid& grid, unsigned int x, unsigned int y) {
	m_entities.push_back({ IDPlant1, vec2(x * 32, y * 32) });

	if (grid(x + 1, y).type != Dungeon::TilePlant)
		m_entities.push_back({ IDPlant2, vec2(x * 32 + 8, y * 32) });

	if (grid(x - 1, y).type != Dungeon::TilePlant)
		m_entities.push_back({ IDPlant2, vec2(x * 32 - 8, y * 32) });
	
	if (grid(x, y + 1).type != Dungeon::TilePlant)
		m_entities.push_back({ IDPlant2, vec2(x * 32, y * 32 + 8) });
	
	if (grid(x, y - 1).type != Dungeon::TilePlant)
		m_entities.push_back({ IDPlant2, vec2(x * 32, y * 32 - 8) });
}

void Level::placeDoor(Dungeon::Grid& grid, unsigned int x, unsigned int y) {
	auto num = 2u;

	if (grid(x + 1, y).type == Dungeon::TileWall && grid(x - 1, y).type == Dungeon::TileWall)
		++num;

	switch (std::rand() % num) {
	case 1:
		m_entities.push_back({ IDBarrel, vec2(x * 32, y * 32) });
		break;
	case 2:
		m_entities.push_back({ IDDoor, vec2(x * 32, y * 32) });
		break;
	}
}
