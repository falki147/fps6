#pragma once

#include <Collisions.hpp>
#include <Level.hpp>
#include <Math/Vector.hpp>
#include <Program/Default.hpp>
#include <Program/Primitive.hpp>
#include <IMG.hpp>
#include <Graphics/Program.hpp>
#include <Graphics/StaticBatch.hpp>
#include <Graphics/StreamedBatch.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/VAO.hpp>
#include <Graphics/VBO.hpp>
#include <Scenes/Scene.hpp>
#include <Graphics/MD2.hpp>
#include <Program/MD2.hpp>
#include <map>
#include <ResourceHandler.hpp>

class MainScene: public Scene {
public:
	MainScene(const Level& level);
	~MainScene();

	void update();
	void render();
	void onKeyDown(SDL_Scancode code, SDL_Keycode key);
	void onKeyUp(SDL_Scancode code, SDL_Keycode key);

	void renderMD2(MD2Collection::MD2ID id, TextureCollection::TextureID tex, float frame, mat4 mat);
	void renderMODB(MODBCollection::MODBID id, TextureCollection::TextureID tex, mat4 mat);
	void renderWall(TextureCollection::TextureID id, vec3 p1, vec3 p2);
	void renderSprite(SpriteCollection::SpriteID id, unsigned int ind, vec2 pos, vec2 scale);
	void renderRectangle(vec2 p1, vec2 p2, unsigned int color);
	void removeMonster();

	class Player* getPlayer() {
		return m_player;
	}

	void setCamera(const vec3& eye, const vec3& target, const vec3& up) {
		m_eye = eye;
		m_target = target;
		m_up = up;
	}

	const vec3& getEye() {
		return m_eye;
	}

	std::map<std::string, bool>& getCheatStatus() {
		return m_cheatStatus;
	}

	const std::vector<Collision::Line>& getCollisionLines() const {
		return m_collisionLines;
	}

	const std::vector<class Entity*>& getEntities() const {
		return m_entities;
	}

private:
	struct Vertex {
		static void prepare(BatchBase<Vertex>& batch);

		vec3 pos;
		vec2 texCoord;
	};

	struct VertexColored {
		static void prepare(BatchBase<VertexColored>& batch);

		vec3 pos;
		unsigned int color;
	};

	struct MD2RenderEntry {
		TextureCollection::TextureID tex;
		float frame;
		mat4 mat;
	};

	struct MODBRenderEntry {
		TextureCollection::TextureID tex;
		mat4 mat;
	};

	void buildLevel(const Level& level);
	void buildWallVert(StaticBatch<MainScene::Vertex>& out, float x, float y);
	void buildWallHor(StaticBatch<MainScene::Vertex>& out, float x, float y);

	std::vector<MD2RenderEntry> m_md2Entries[MD2Collection::MD2Count];
	std::vector<MODBRenderEntry> m_modbEntries[MODBCollection::MODBCount];

	StaticBatch<Vertex>& getStaticBatch(TextureCollection::TextureID id);
	StreamedBatch<Vertex>& getStreamedBatch(TextureCollection::TextureID id);
	StreamedBatch<Vertex>& getStreamedSpriteBatch(SpriteCollection::SpriteID id);

	unsigned int m_monsters = 0;
	std::string m_cheatString;

	class Player* m_player = nullptr;
	std::vector<class Entity*> m_entities;

	std::vector<Collision::Line> m_collisionLines;
	std::map<std::string, bool> m_cheatStatus;

	std::map<TextureCollection::TextureID, StaticBatch<Vertex>> m_static;
	std::map<TextureCollection::TextureID, StreamedBatch<Vertex>> m_streamed;
	std::map<SpriteCollection::SpriteID, StreamedBatch<Vertex>> m_spritesStreamed;
	StreamedBatch<VertexColored> m_primitives;

	vec3 m_eye, m_target, m_up;
	DefaultProgram m_progDefault;
	PrimitiveProgram m_progPrimitive;
	MD2Program m_progMD2;
};
