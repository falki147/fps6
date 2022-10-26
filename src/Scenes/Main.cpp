#include <Entities/Barrel.hpp>
#include <Entities/Door.hpp>
#include <Entities/Monster.hpp>
#include <Entities/Plant1.hpp>
#include <Entities/Plant2.hpp>
#include <Entities/Player.hpp>
#include <fstream>
#include <Level.hpp>
#include <Window.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <Scenes/Main.hpp>
#include <SDL.h>
#include <Game.hpp>

static void checkCollisions(Entity* entity, const std::vector<Collision::Line>& lines, const std::vector<Entity*>& entities) {
	const int maxCollisions = 10;
	
	if (entity->getObject().type != Collision::Object::TypeCircle)
		return;
	
	auto circle = entity->getObject().circle;
	
	for (auto& line : lines)
		if (Collision::collides(circle, line))
			Collision::resolve(circle, line);
	
	for (auto other : entities)
		if (other != entity && other->isCollidable() && Collision::collides(circle, other->getObject()))
			Collision::resolve(circle, other->getObject());

	entity->setPosition(circle.pos);
}

MainScene::MainScene(const Level& level) {
	Game::getResourceHandler()->getSound()[SoundCollection::SoundQuake].play(true);

	m_cheatStatus.emplace("FLY", false);
	m_cheatStatus.emplace("HESOYAM", false);
	m_cheatStatus.emplace("BAGOFIX", false);
	m_cheatStatus.emplace("CHAIN", false);
	m_cheatStatus.emplace("FORK", false);
	m_cheatStatus.emplace("IWANT3D", false);
	m_cheatStatus.emplace("HARDSTYLE", false);

	Window::enableMouseWarp(true);

	VertexColored::prepare(m_primitives);

	buildLevel(level);
}

MainScene::~MainScene() {
	if (m_player)
		delete m_player;

	for (auto entity : m_entities)
		delete entity;

	Window::enableMouseWarp(false);
	Game::getResourceHandler()->getSound()[SoundCollection::SoundQuake].stop();
	Game::getResourceHandler()->getSound()[SoundCollection::SoundHardstyle].stop();
}

void MainScene::renderMD2(MD2Collection::MD2ID id, TextureCollection::TextureID tex, float frame, mat4 mat) {
	m_md2Entries[id].push_back({ tex, frame, mat });
}

void MainScene::renderMODB(MODBCollection::MODBID id, TextureCollection::TextureID tex, mat4 mat) {
	m_modbEntries[id].push_back({ tex, mat });
}

void MainScene::renderWall(TextureCollection::TextureID id, vec3 p1, vec3 p2) {
	auto& dest = getStreamedBatch(id);

	dest.add({ vec3(p1.x, p1.y, p1.z), vec2(0, 0) });
	dest.add({ vec3(p1.x, p1.y, p2.z), vec2(0, 1) });
	dest.add({ vec3(p2.x, p2.y, p1.z), vec2(1, 0) });
	dest.add({ vec3(p2.x, p2.y, p1.z), vec2(1, 0) });
	dest.add({ vec3(p2.x, p2.y, p2.z), vec2(1, 1) });
	dest.add({ vec3(p1.x, p1.y, p2.z), vec2(0, 1) });
}

void MainScene::renderSprite(SpriteCollection::SpriteID id, unsigned int ind, vec2 pos, vec2 scale) {
	auto& dest = getStreamedSpriteBatch(id);
	auto spr = Game::getResourceHandler()->getSprite().getInfo(id, ind);

	auto p1 = pos + vec2(spr.offset.x * scale.x, spr.offset.y * scale.y);
	auto p2 = p1 + vec2(spr.size.x * scale.x, spr.size.y * scale.y);

	dest.add({ vec3(p1.x, p1.y, 0), vec2(spr.p1.x, spr.p1.y) });
	dest.add({ vec3(p1.x, p2.y, 0), vec2(spr.p1.x, spr.p2.y) });
	dest.add({ vec3(p2.x, p1.y, 0), vec2(spr.p2.x, spr.p1.y) });
	dest.add({ vec3(p2.x, p1.y, 0), vec2(spr.p2.x, spr.p1.y) });
	dest.add({ vec3(p2.x, p2.y, 0), vec2(spr.p2.x, spr.p2.y) });
	dest.add({ vec3(p1.x, p2.y, 0), vec2(spr.p1.x, spr.p2.y) });
}

void MainScene::renderRectangle(vec2 p1, vec2 p2, unsigned int color) {
	m_primitives.add({ vec3(p1.x, p1.y, 0), color });
	m_primitives.add({ vec3(p1.x, p2.y, 0), color });
	m_primitives.add({ vec3(p2.x, p1.y, 0), color });
	m_primitives.add({ vec3(p2.x, p1.y, 0), color });
	m_primitives.add({ vec3(p2.x, p2.y, 0), color });
	m_primitives.add({ vec3(p1.x, p2.y, 0), color });
}

void MainScene::onKeyDown(SDL_Scancode code, SDL_Keycode key) {
	const auto largestLength = 10;
	auto& sounds = Game::getResourceHandler()->getSound();

	if (key == SDLK_m) {
		auto music = m_cheatStatus["HARDSTYLE"] ? SoundCollection::SoundHardstyle : SoundCollection::SoundQuake;

		if (sounds[music].isPlaying())
			sounds[music].stop();
		else
			sounds[music].play(true);
	}

	char chr = 0;

	if (key >= SDLK_a && key <= SDLK_z)
		chr = key + 'A' - SDLK_a;
	else if (key >= SDLK_0 && key <= SDLK_9)
		chr = key + '0' - SDLK_0;

	if (chr != 0) {
		if (m_cheatString.size() >= largestLength)
			m_cheatString.erase(m_cheatString.begin());

		m_cheatString.push_back(chr);

		for (auto& entry : m_cheatStatus) {
			if (entry.first.size() > m_cheatString.size())
				continue;

			if (entry.first == m_cheatString.c_str() + (m_cheatString.size() - entry.first.size())) {
				if (entry.second) {
					entry.second = false;
					SDL_ShowSimpleMessageBox(0, "yeeh motherfucker", "Cheat deactivated.", nullptr);

					if (entry.first == "HARDSTYLE") {
						sounds[SoundCollection::SoundHardstyle].stop();
						sounds[SoundCollection::SoundQuake].play(true);
					}
				}
				else {
					entry.second = true;
					SDL_ShowSimpleMessageBox(0, "yeeh motherfucker", "You entered a cheat by dannell", nullptr);

					if (entry.first == "HARDSTYLE") {
						sounds[SoundCollection::SoundQuake].stop();
						sounds[SoundCollection::SoundHardstyle].play(true);
					}
				}
			}
		}
	}
}

void MainScene::onKeyUp(SDL_Scancode code, SDL_Keycode key) {
	
}

void MainScene::removeMonster() {
	--m_monsters;

	if (m_monsters == 0) {
		SDL_ShowSimpleMessageBox(0, "fps6", "Your winner!", nullptr);
		Game::Leave();
	}
}

void MainScene::update() {
	// Update entities
	if (m_player)
		m_player->update(*this);

	for (auto entity : m_entities)
		entity->update(*this);

	// Check and resolve collisions
	if (!m_cheatStatus["FLY"] && m_player->hasMoved())
		checkCollisions(m_player, m_collisionLines, m_entities);

	for (auto entity : m_entities)
		if (entity->hasMoved())
			checkCollisions(entity, m_collisionLines, m_entities);

	// Render entites to batch
	m_player->render(*this);

	for (auto entity : m_entities)
		entity->render(*this);

	// Upload batch data
	for (auto& streamed : m_streamed)
		streamed.second.upload();

	for (auto& streamed : m_spritesStreamed)
		streamed.second.upload();

	m_primitives.upload();
}

void MainScene::render() {
	auto matPerspective = mat4::createPerspective(1.57f, 640.f / 480.f, 1, 300.0f);
	auto matLookAt = mat4::createLookAt(m_eye, m_target, m_up);
	auto mat = matPerspective * matLookAt;
	auto& textures = Game::getResourceHandler()->getTexture();
	auto& md2 = Game::getResourceHandler()->getMD2();
	auto& modb = Game::getResourceHandler()->getMODB();
	auto& sprites = Game::getResourceHandler()->getSprite();

	m_progDefault.use();
	m_progDefault.setMVP(mat);

	for (auto& batch : m_static) {
		textures[batch.first].bindStage(0);
		batch.second.render();
	}

	for (auto& batch : m_streamed) {
		textures[batch.first].bindStage(0);
		batch.second.render();
	}

	for (auto i = 0u; i < MODBCollection::MODBCount; ++i) {
		for (auto& entry : m_modbEntries[i]) {
			m_progDefault.setMVP(mat * entry.mat);
			textures[entry.tex].bindStage(0);
			modb[(MODBCollection::MODBID) i].render();
		}

		m_modbEntries[i].clear();
	}

	m_progMD2.use();
	
	for (auto i = 0u; i < MD2Collection::MD2Count; ++i) {
		for (auto& entry : m_md2Entries[i]) {
			m_progMD2.setMVP(mat * entry.mat);
			m_progMD2.setWeight(entry.frame - std::floor(entry.frame));
			textures[entry.tex].bindStage(0);
			md2[(MD2Collection::MD2ID) i].render((unsigned int) entry.frame);
		}

		m_md2Entries[i].clear();
	}

	mat = mat4::createOrthographic(0, 640, 480, 0, -1, 1);
	m_progDefault.use();
	m_progDefault.setMVP(mat);

	glDisable(GL_DEPTH_TEST);

	for (auto& batch : m_spritesStreamed) {
		sprites[batch.first].bindStage(0);
		batch.second.render();
	}

	m_progPrimitive.use();
	m_progPrimitive.setMVP(mat);
	m_primitives.render();

	glEnable(GL_DEPTH_TEST);
}

void MainScene::buildLevel(const Level& level) {
	struct Info {
		bool isVertical;
		TextureCollection::TextureID texture;
	};

	Info table[] = {
		{ false, TextureCollection::TextureWall1 },
		{ false, TextureCollection::TextureWall2 },
		{ false, TextureCollection::TextureWall3 },
		{ false, TextureCollection::TextureWall4 },
		{ false, TextureCollection::TextureWall5 },
		{ false, TextureCollection::TextureWall6 },
		{ true,  TextureCollection::TextureWall1 },
		{ true,  TextureCollection::TextureWall2 },
		{ true,  TextureCollection::TextureWall3 },
		{ true,  TextureCollection::TextureWall4 },
		{ true,  TextureCollection::TextureWall5 },
		{ true,  TextureCollection::TextureWall6 }
	};

	auto minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max();
	auto maxX = -std::numeric_limits<float>::max(), maxY = -std::numeric_limits<float>::max();

	for (auto& entity : level.getEntities()) {
		switch (entity.id) {
		case Level::IDPlayer:
			if (m_player)
				continue;

			m_player = new Player(entity.pos);
			break;
		case Level::IDDoor:
			m_entities.push_back(new Door(entity.pos));
			break;
		case Level::IDBarrel:
			m_entities.push_back(new Barrel(entity.pos));
			break;
		case Level::IDMonster1:
			m_entities.push_back(new Monster(entity.pos));
			++m_monsters;
			break;
		case Level::IDPlant1:
			m_entities.push_back(new Plant1(entity.pos));
			break;
		case Level::IDPlant2:
			m_entities.push_back(new Plant2(entity.pos));
			break;
		default:
			if (entity.id >= Level::IDWall1Hor && entity.id <= Level::IDWall6Vert) {
				auto& info = table[entity.id - 1];

				if (info.isVertical) {
					buildWallVert(getStaticBatch(info.texture), entity.pos.x, entity.pos.y);

					if (entity.pos.x < minX)
						minX = entity.pos.x;

					if (entity.pos.y - 16 < minY)
						minY = entity.pos.y - 16;

					if (maxX < entity.pos.x)
						maxX = entity.pos.x;

					if (maxY < entity.pos.y + 16)
						maxY = entity.pos.y + 16;
				}
				else {
					buildWallHor(getStaticBatch(info.texture), entity.pos.x, entity.pos.y);

					if (entity.pos.x - 16 < minX)
						minX = entity.pos.x - 16;

					if (entity.pos.y < minY)
						minY = entity.pos.y;

					if (maxX < entity.pos.x + 16)
						maxX = entity.pos.x + 16;

					if (maxY < entity.pos.y)
						maxY = entity.pos.y;
				}
			}
		}
	}

	auto& floor = getStaticBatch(TextureCollection::TextureFloor);

	floor.add({ vec3(minX, minY,  0), vec2(0, 0) });
	floor.add({ vec3(maxX, minY,  0), vec2((maxX - minX) / 32, 0) });
	floor.add({ vec3(minX, maxY,  0), vec2(0, (maxY - minY) / 32) });
	floor.add({ vec3(minX, maxY,  0), vec2(0, (maxY - minY) / 32) });
	floor.add({ vec3(maxX, maxY,  0), vec2((maxX - minX) / 32, (maxY - minY) / 32) });
	floor.add({ vec3(maxX, minY,  0), vec2((maxX - minX) / 32, 0) });

	auto& ceiling = getStaticBatch(TextureCollection::TextureCeiling);

	ceiling.add({ vec3(minX, minY,  32), vec2(0, 0) });
	ceiling.add({ vec3(maxX, minY,  32), vec2((maxX - minX) / 32, 0) });
	ceiling.add({ vec3(minX, maxY,  32), vec2(0, (maxY - minY) / 32) });
	ceiling.add({ vec3(minX, maxY,  32), vec2(0, (maxY - minY) / 32) });
	ceiling.add({ vec3(maxX, maxY,  32), vec2((maxX - minX) / 32, (maxY - minY) / 32) });
	ceiling.add({ vec3(maxX, minY,  32), vec2((maxX - minX) / 32, 0) });

	for (auto& batch : m_static)
		batch.second.upload();
}

void MainScene::buildWallVert(StaticBatch<MainScene::Vertex>& out, float x, float y) {
	out.add({ vec3(x, y - 16,  0), vec2(0, 0) });
	out.add({ vec3(x, y - 16, 32), vec2(0, 1) });
	out.add({ vec3(x, y + 16,  0), vec2(1, 0) });
	out.add({ vec3(x, y + 16,  0), vec2(1, 0) });
	out.add({ vec3(x, y + 16, 32), vec2(1, 1) });
	out.add({ vec3(x, y - 16, 32), vec2(0, 1) });

	m_collisionLines.push_back({ vec2(x, y - 16), vec2(x, y + 16) });
}

void MainScene::buildWallHor(StaticBatch<MainScene::Vertex>& out, float x, float y) {
	out.add({ vec3(x - 16, y,  0), vec2(0, 0) });
	out.add({ vec3(x - 16, y, 32), vec2(0, 1) });
	out.add({ vec3(x + 16, y,  0), vec2(1, 0) });
	out.add({ vec3(x + 16, y,  0), vec2(1, 0) });
	out.add({ vec3(x + 16, y, 32), vec2(1, 1) });
	out.add({ vec3(x - 16, y, 32), vec2(0, 1) });

	m_collisionLines.push_back({ vec2(x - 16, y), vec2(x + 16, y) });
}

StaticBatch<MainScene::Vertex>& MainScene::getStaticBatch(TextureCollection::TextureID id) {
	auto it = m_static.find(id);

	if (it != m_static.end())
		return it->second;

	auto& batch = m_static[id];
	Vertex::prepare(batch);
	return batch;
}

StreamedBatch<MainScene::Vertex>& MainScene::getStreamedBatch(TextureCollection::TextureID id) {
	auto it = m_streamed.find(id);

	if (it != m_streamed.end())
		return it->second;

	auto& batch = m_streamed[id];
	Vertex::prepare(batch);
	return batch;
}

StreamedBatch<MainScene::Vertex>& MainScene::getStreamedSpriteBatch(SpriteCollection::SpriteID id) {
	auto it = m_spritesStreamed.find(id);

	if (it != m_spritesStreamed.end())
		return it->second;

	auto& batch = m_spritesStreamed[id];
	Vertex::prepare(batch);
	return batch;
}

void MainScene::Vertex::prepare(BatchBase<Vertex>& batch) {
	batch.beginAttributes();
	batch.attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	batch.attribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 12);
	batch.endAttributes();
}

void MainScene::VertexColored::prepare(BatchBase<VertexColored>& batch) {
	batch.beginAttributes();
	batch.attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColored), 0);
	batch.attribute(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexColored), 12);
	batch.endAttributes();
}
