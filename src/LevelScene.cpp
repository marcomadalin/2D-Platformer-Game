#include "LevelScene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

LevelScene::LevelScene(string fn) : Scene(fn),  map{ nullptr } {}

LevelScene::~LevelScene()
{
	map->free();
}

void LevelScene::init() {
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	texProgram = ShaderProgram();
	initShaders();
	if (map != nullptr) map->free();
	winCounter = 0;

	transfering1 = transfering2 = remove = false;
	firstPress = true;

	imageTex.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	image = Sprite::createSprite(glm::ivec2(1024, 640
	), glm::vec2(1.f, 1.f), &imageTex, &texProgram);
	image->setPosition(glm::vec2(0.f, 0.f));

	map = TileMap::createTileMap("levels/" + fileName, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player1.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/pinkPlayer.png");
	player1.setPosition(glm::vec2(map->getInitPos(PLAYER1).x * map->getTileSize(), map->getInitPos(PLAYER1).y * map->getTileSize()));
	player1.setTileMap(map);

	player2.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/whitePlayer.png");
	player2.setPosition(glm::vec2(map->getInitPos(PLAYER2).x * map->getTileSize(), map->getInitPos(PLAYER2).y * map->getTileSize()));
	player2.setTileMap(map);

	coin1.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	coin1.setPosition(glm::vec2(map->getInitPos(COIN1).x * map->getTileSize(), map->getInitPos(COIN1).y * map->getTileSize()));

	coin2.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	coin2.setPosition(glm::vec2(map->getInitPos(COIN2).x * map->getTileSize(), map->getInitPos(COIN2).y * map->getTileSize()));

	chest1.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/chest.png");
	chest1.setPosition(glm::vec2(map->getInitPos(CHEST1).x * map->getTileSize(), map->getInitPos(CHEST1).y * map->getTileSize()));

	chest2.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/chest2.png");
	chest2.setPosition(glm::vec2(map->getInitPos(CHEST2).x * map->getTileSize(), map->getInitPos(CHEST2).y * map->getTileSize()));

	if (map->getInitPos(BUTTON).x != -1) {
		button.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/button.png");
		button.setPosition(glm::vec2(map->getInitPos(BUTTON).x * map->getTileSize(), map->getInitPos(BUTTON).y * map->getTileSize()));
	}
	if (map->getInitPos(WALL).x != -1) {
		wall.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/button.png");
		wall.setPosition(glm::vec2(map->getInitPos(WALL).x * map->getTileSize(), map->getInitPos(WALL).y * map->getTileSize()));
	}
	waterBlocks = vector<Water*>(map->getMapSize().x);
	inversedWaterBlocks = vector<Water*>(map->getMapSize().x);
	for (int i = 0; i < waterBlocks.size(); ++i) {
		waterBlocks[i] = new Water();
		waterBlocks[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/water.png", i % 4);
		waterBlocks[i]->setPosition(glm::vec2(i * map->getTileSize(), (((map->getMapSize().y - 1) / 2) - 1) * map->getTileSize()));

		inversedWaterBlocks[i] = new Water();
		inversedWaterBlocks[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/water2.png", i % 4);
		inversedWaterBlocks[i]->setPosition(glm::vec2(i * map->getTileSize(), (((map->getMapSize().y - 1) / 2) + 2) * map->getTileSize()));
	}

	if (map->getInitPos(PORTALIN).x != -1) {
		portalIn.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/portal.png", 0);
		portalIn.setPosition(glm::vec2(map->getInitPos(PORTALIN).x * map->getTileSize(), map->getInitPos(PORTALIN).y * map->getTileSize()));
		portalOut.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/portal.png", 2);
		portalOut.setPosition(glm::vec2(map->getInitPos(PORTALOUT).x * map->getTileSize(), map->getInitPos(PORTALOUT).y * map->getTileSize()));
	}

	if (map->getInitPos(INVERTEDBOX).x != -1) {
		box.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "images/box.png");
		box.setPosition(glm::vec2(map->getInitPos(INVERTEDBOX).x * map->getTileSize(), map->getInitPos(INVERTEDBOX).y * map->getTileSize()));
		box.setTileMap(map);
	}
}

void LevelScene::update(int deltaTime)
{
	currentTime += deltaTime;

	for (auto i : waterBlocks) i->update(deltaTime);
	for (auto i : inversedWaterBlocks) i->update(deltaTime);
	bool pressed = remove;
	Game& g = Game::instance();
	g.lastLevelScene = g.currentScene;
	if (map->getInitPos(PORTALIN).x != -1) {
		transfering1 = portalIn.update(deltaTime, player1.getPosition(), transfering2);
		if (transfering1) g.soundtrack.playPortal();
		transfering2 = portalOut.update(deltaTime, player1.getPosition(), transfering1);
		if (transfering2) g.soundtrack.playPortal();
	}
	if (map->getInitPos(BUTTON).x != -1) {
		pressed = button.update(deltaTime, player1.getPosition());
		if (pressed && firstPress) {
			g.soundtrack.playButton();
			firstPress = false;
		}
		pressed = pressed || remove;

	}
	player1.update(deltaTime, (winCounter != 0), transfering1, transfering2, map->getInitPos(PORTALIN) * map->getTileSize(), map->getInitPos(PORTALOUT) * map->getTileSize());
	player2.updateInversed(deltaTime, (winCounter != 0), map->getInitPos(WALL) * map->getTileSize(), !pressed, box.getPosition());
	if (map->getInitPos(INVERTEDBOX).x != -1)
		box.update(deltaTime, player2.getPosition(), !pressed, map->getInitPos(WALL) * map->getTileSize());
	coin1.update(deltaTime, player1.coinCollected());
	coin2.update(deltaTime, player2.coinCollected());
	chest1.update(deltaTime, (player1.isInGoal() && player2.isInGoal()));
	chest2.update(deltaTime, (player1.isInGoal() && player2.isInGoal()));
	if (map->getInitPos(WALL).x != -1) wall.update(deltaTime, pressed);

	if (g.superUser || (player1.isAlive() && player2.isAlive())) {
		if (player1.isInGoal() && player2.isInGoal()) {
			++winCounter;
			if (winCounter >= 30) {
				g.soundtrack.stopWalk();
				g.soundtrack.playLevelPassed();
				g.currentScene = Game::scenes::LEVELCOMPLETED;
			}
		}
	}
	else if (!g.superUser) {
		g.soundtrack.stopWalk();
		g.soundtrack.stopMusic();
		g.soundtrack.playGameOver();
		g.currentScene = Game::scenes::GAMEOVER;
	}
}

void LevelScene::render()
{
	float camerax0 = player1.getPosition()[0] - float(SCREEN_WIDTH - 1.f) / 2.f;
	float cameraxf = player1.getPosition()[0] + float(SCREEN_WIDTH - 1.f) / 2.f;

	if (camerax0 < 0.f) {
		camerax0 = 0;
		cameraxf = camerax0 + float(SCREEN_WIDTH - 1.f);
	}
	if (cameraxf > 960.f) {
		cameraxf = 960.f;
		camerax0 = cameraxf - float(SCREEN_WIDTH - 1.f);
	}
	projection = glm::ortho(camerax0, cameraxf, float(112.f + SCREEN_HEIGHT - 1.f), 112.f);

	image->render();
	map->render();
	if (map->getInitPos(PORTALIN).x != -1) {
		portalIn.render();
		portalOut.render();
	}
	player1.render();
	player2.render();
	if (map->getInitPos(INVERTEDBOX).x != -1) box.render();
	coin1.render();
	coin2.render();
	chest1.render();
	chest2.render();
	if (map->getInitPos(BUTTON).x != -1) button.render();
	if (map->getInitPos(WALL).x != -1) wall.render();
	for (auto i : waterBlocks) i->render();
	for (auto i : inversedWaterBlocks) i->render();
	setTexProgram();
}

void LevelScene::removeWall()
{
	remove = true;
}
