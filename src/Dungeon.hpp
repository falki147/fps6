#pragma once

#include <cassert>
#include <vector>

namespace Dungeon {
	struct Config {
		unsigned int width;
		unsigned int height;
		unsigned int numTries;
		unsigned int roomExtraSize;
		float windingPercent;
		float extraConnectorChance;
	};

	enum TileType {
		TileWall,
		TileFloor,
		TileDoor,
		TileStart,
		TileMonster,
		TilePlant,
		TileRoom
	};

	struct Tile {
		TileType type;
		unsigned int region;
	};

	class Grid {
	public:
		Grid() { }

		Grid(unsigned int width, unsigned int height):
			m_width(width), m_height(height), m_data(width * height) { }

		Tile& operator()(unsigned int i, unsigned int j) {
			assert(i < m_width && j < m_height);
			return m_data[j * m_width + i];
		}

		const Tile& operator()(unsigned int i, unsigned int j) const {
			assert(i < m_width && j < m_height);
			return m_data[j * m_width + i];
		}

		std::vector<Tile>& data() {
			return m_data;
		}

		unsigned int width() const {
			return m_width;
		}

		unsigned int height() const {
			return m_height;
		}

	private:
		unsigned int m_width, m_height;
		std::vector<Tile> m_data;
	};

	Grid create(const Config& conf);
}
