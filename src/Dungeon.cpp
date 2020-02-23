#include "Dungeon.hpp"

#include <algorithm>
#include <array>

struct Room {
	unsigned int x, y;
	unsigned int width, height;
};

struct DungeonData {
	Dungeon::Config conf;
	Dungeon::Grid grid;
	unsigned int currentRegion;
	std::vector<Room> rooms;
};

struct Point {
	unsigned int x, y;
};

static void addRooms(DungeonData& dungeon);
static void growMaze(DungeonData& dungeon, unsigned int x, unsigned int y);
static void connectRegions(DungeonData& dungeon);
static void removeDeadEnds(DungeonData& dungeon);
static void placeStart(DungeonData& dungeon);
static void placeMonster(DungeonData& dungeon);
static void placeDoor(DungeonData& dungeon, unsigned int x, unsigned int y);
static void carveRooms(DungeonData& dungeon);

static const unsigned int offsetX[4] = { 2, 1, 0, 1 };
static const unsigned int offsetY[4] = { 1, 0, 1, 2 };

inline unsigned int rand(unsigned int max) {
	return std::rand() % max;
}

inline unsigned int rand(unsigned int min, unsigned int max) {
	return std::rand() % (max - min) + min;
}

inline float randF() {
	return std::rand() / (float) RAND_MAX;
}

Dungeon::Grid Dungeon::create(const Config& conf) {
	if (conf.width % 2 == 0 || conf.height % 2 == 0)
		throw std::runtime_error("width and height must be odd numbers");

	DungeonData dungeon = {
		conf, Grid(conf.width, conf.height), (unsigned int) -1, std::vector<Room>()
	};

	addRooms(dungeon);

	for (auto j = 1u; j < dungeon.conf.height; j += 2) {
		for (auto i = 1u; i < dungeon.conf.width; i += 2) {
			if (dungeon.grid(i, j).type != TileWall)
				continue;

			growMaze(dungeon, i, j);
		}
	}

	connectRegions(dungeon);
	removeDeadEnds(dungeon);
	placeStart(dungeon);
	carveRooms(dungeon);
	placeMonster(dungeon);

	return dungeon.grid;
}

void addRooms(DungeonData& dungeon) {
	for (auto i = 0u; i < dungeon.conf.numTries; ++i) {
		auto size = rand(1, 3 + dungeon.conf.roomExtraSize) * 2 + 1;
		auto rect = rand(1 + size / 2) * 2;
		
		auto width = size;
		auto height = size;

		if (rand(2))
			width += rect;
		else
			height += rect;

		auto x = rand((dungeon.conf.width - width) / 2) * 2 + 1;
		auto y = rand((dungeon.conf.height - height) / 2) * 2 + 1;

		auto overlaps = false;
		for (auto& room : dungeon.rooms)
			if (x < room.x + room.width && x + width > room.x && y < room.y + room.height && height + y > room.y)
				overlaps = true;

		if (overlaps)
			continue;

		dungeon.rooms.push_back({ x, y, width, height });
		++dungeon.currentRegion;

		for (auto j = y; j < y + height; ++j)
			for (auto i = x; i < x + width; ++i)
				dungeon.grid(i, j) = { Dungeon::TileRoom, dungeon.currentRegion };
	}
}

void growMaze(DungeonData& dungeon, unsigned int x, unsigned int y) {
	std::vector<Point> cells;
	unsigned int lastDir = -1;

	++dungeon.currentRegion;
	dungeon.grid(x, y) = { Dungeon::TileFloor, dungeon.currentRegion };

	cells.push_back({ x, y });

	while (!cells.empty()) {
		auto cell = cells.back();
		
		unsigned int directions[4];
		bool directionAvailable[4] = { };
		auto numDirections = 0u;
		
		for (auto i = 0u; i < 4; ++i) {
			// Check if the cell (cell + offset * 3 - 3) is inside the rectangle
			if (cell.x + offsetX[i] * 3 < 3 || cell.y + offsetY[i] * 3 < 3)
				continue;

			if (cell.x + offsetX[i] * 3 - 3 >= dungeon.conf.width || cell.y + offsetY[i] * 3 - 3 >= dungeon.conf.height)
				continue;

			// Check if the cell (cell + offset * 2 - 2) is a wall
			if (dungeon.grid(cell.x + offsetX[i] * 2 - 2, cell.y + offsetY[i] * 2 - 2).type != Dungeon::TileWall)
				continue;

			directionAvailable[i] = true;
			directions[numDirections++] = i;
		}

		if (numDirections != 0) {
			unsigned int dir;

			if (lastDir != -1 && directionAvailable[lastDir] && randF() > dungeon.conf.windingPercent)
				dir = lastDir;
			else
				dir = directions[rand(numDirections)];

			dungeon.grid(cell.x + offsetX[dir] - 1, cell.y + offsetY[dir] - 1) = { Dungeon::TileFloor, dungeon.currentRegion };
			dungeon.grid(cell.x + offsetX[dir] * 2 - 2, cell.y + offsetY[dir] * 2 - 2) = { Dungeon::TileFloor, dungeon.currentRegion };

			cells.push_back({ cell.x + offsetX[dir] * 2 - 2, cell.y + offsetY[dir] * 2 - 2 });
			lastDir = dir;
		}
		else {
			cells.pop_back();
			lastDir = -1;
		}
	}
}

void connectRegions(DungeonData& dungeon) {
	const auto numRegions = dungeon.currentRegion + 1;

	struct TileInfo {
		Point p;
		unsigned int numRegions;
		unsigned int regions[4];
	};

	std::vector<TileInfo> tiles;

	for (auto j = 0u; j < dungeon.conf.width; ++j) {
		for (auto i = 0u; i < dungeon.conf.height; ++i) {
			if (dungeon.grid(i, j).type != Dungeon::TileWall)
				continue;

			TileInfo info;
			info.numRegions = 0;
			
			for (auto k = 0u; k < 4; ++k) {
				// Check if the cell (cell + offset) is inside the rectangle
				if (i + offsetX[k] < 1 || j + offsetY[k] < 1)
					continue;

				if (i + offsetX[k] - 1 >= dungeon.conf.width || j + offsetY[k] - 1 >= dungeon.conf.height)
					continue;

				// Skip walls
				if (dungeon.grid(i + offsetX[k] - 1, j + offsetY[k] - 1).type == Dungeon::TileWall)
					continue;

				auto region = dungeon.grid(i + offsetX[k] - 1, j + offsetY[k] - 1).region;
				auto found = false;

				for (auto l = 0u; l < info.numRegions; ++l) {
					if (info.regions[l] == region) {
						found = true;
						break;
					}
				}

				if (!found)
					info.regions[info.numRegions++] = region;
			}

			if (info.numRegions < 2)
				continue;

			info.p = { i, j };
			tiles.push_back(info);
		}
	}

	std::vector<unsigned int> merged(numRegions);
	std::vector<unsigned int> openRegions(numRegions);

	for (auto i = 0u; i < numRegions; ++i) {
		merged[i] = i;
		openRegions[i] = i;
	}

	while (openRegions.size() > 1 && !tiles.empty()) {
		auto connector = tiles[rand(tiles.size())];

		placeDoor(dungeon, connector.p.x, connector.p.y);

		unsigned int mergedRegion[4];

		for (auto i = 0u; i < connector.numRegions; ++i)
			mergedRegion[i] = merged[connector.regions[i]];

		for (auto i = 0u; i < numRegions; ++i) {
			for (auto j = 1u; j < connector.numRegions; ++j) {
				if (mergedRegion[j] == merged[i]) {
					merged[i] = mergedRegion[0];
					break;
				}
			}
		}

		openRegions.erase(std::remove_if(openRegions.begin(), openRegions.end(), [&mergedRegion, &connector](auto region) {
			for (auto j = 1u; j < connector.numRegions; ++j)
				if (mergedRegion[j] == region)
					return true;

			return false;
		}), openRegions.end());

		tiles.erase(std::remove_if(tiles.begin(), tiles.end(), [&merged, &connector, &dungeon](auto tile) {
			if (std::max(connector.p.x, tile.p.x) - std::min(connector.p.x, tile.p.x) <= 1 && std::max(connector.p.y, tile.p.y) - std::min(connector.p.y, tile.p.y) <= 1)
				return true;

			auto numUnique = 0u;
			unsigned int uniqueRegions[4];

			for (auto i = 0u; i < tile.numRegions; ++i) {
				auto region = merged[tile.regions[i]];
				auto found = false;

				for (auto j = 0u; j < numUnique; ++j) {
					if (uniqueRegions[j] == region) {
						found = true;
						break;
					}
				}

				if (found)
					continue;

				uniqueRegions[numUnique++] = region;
			}

			if (numUnique > 1)
				return false;

			auto found = false;

			for (auto i = 0u; i < 4; ++i) {
				if (dungeon.grid(tile.p.x + offsetX[i] - 1, tile.p.y + offsetY[i] - 1).type == Dungeon::TileDoor) {
					found = true;
					break;
				}
			}

			if (!found && randF() < dungeon.conf.extraConnectorChance)
				placeDoor(dungeon, tile.p.x, tile.p.y);

			return true;
		}), tiles.end());
	}
}

void removeDeadEnds(DungeonData& dungeon) {
	auto done = false;

	while (!done) {
		done = true;

		for (auto j = 0u; j < dungeon.conf.width; ++j) {
			for (auto i = 0u; i < dungeon.conf.height; ++i) {
				if (dungeon.grid(i, j).type == Dungeon::TileWall)
					continue;

				auto exits = 0;

				for (auto k = 0u; k < 4; ++k)
					if (dungeon.grid(i + offsetX[k] - 1, j + offsetY[k] - 1).type != Dungeon::TileWall)
						++exits;

				if (exits > 1)
					continue;

				done = false;
				dungeon.grid(i, j).type = Dungeon::TileWall;
			}
		}
	}
}

void placeStart(DungeonData& dungeon) {
	unsigned int smallest;
	unsigned int smallestArea = -1;

	for (auto i = 0u; i < dungeon.rooms.size(); ++i) {
		if (dungeon.rooms[i].width * dungeon.rooms[i].height < smallestArea) {
			smallest = i;
			smallestArea = dungeon.rooms[i].width * dungeon.rooms[i].height;
		}
	}

	if (smallestArea == -1)
		throw std::runtime_error("failed to place start");

	dungeon.grid(dungeon.rooms[smallest].x + dungeon.rooms[smallest].width / 2, dungeon.rooms[smallest].y + dungeon.rooms[smallest].height / 2).type = Dungeon::TileStart;
	dungeon.rooms.erase(dungeon.rooms.begin() + smallest);
}

void placeMonster(DungeonData& dungeon) {
	for (auto& room : dungeon.rooms) {
		auto numMonsters = room.width * room.height / 4;

		for (auto i = 0u; i < numMonsters; ++i) {
			auto& cell = dungeon.grid(room.x + rand(room.width), room.y + rand(room.height));

			if (cell.type == Dungeon::TileRoom)
				cell.type = Dungeon::TileMonster;
		}
	}
}

void placeDoor(DungeonData& dungeon, unsigned int x, unsigned int y) {
	unsigned int regions[2];
	unsigned int numRegions = 0;

	for (auto i = 0u; i < 4 && numRegions < 2; ++i) {
		auto& tile = dungeon.grid(x + offsetX[i] - 1, y + offsetY[i] - 1);

		if (tile.type != Dungeon::TileWall)
			regions[numRegions++] = tile.region;
	}

	if (numRegions < 2)
		return;

	dungeon.grid(x, y) = { Dungeon::TileDoor, std::max(regions[0], regions[1]) };
}

void carveRooms(DungeonData& dungeon) {
	for (auto& room : dungeon.rooms) {
		auto roomType = rand(3);

		switch (roomType) {
		case 0:
		case 1:
			auto tileType = roomType == 0 ? Dungeon::TileWall : Dungeon::TilePlant;

			auto width = room.width / 3;
			auto height = room.height / 3;

			auto offsetX = (room.width + 2) / 3;
			auto offsetY = (room.height + 2) / 3;

			for (auto j = 0u; j < height; ++j)
				for (auto i = 0u; i < width; ++i)
					dungeon.grid(room.x + offsetX + i, room.y + offsetY + j).type = tileType;

			break;
		}
	}
}
