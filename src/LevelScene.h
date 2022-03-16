#pragma once
#include "Scene.h"
#include "Game.h"
#include "Coin.h"
#include "Chest.h"
#include "Water.h"
#include "Button.h"
#include "Wall.h"
#include "Portal.h"
#include "InvertedBox.h"

#define SCREEN_X 0
#define SCREEN_Y 0

class LevelScene : public Scene
{
public:
	LevelScene(string fn);
	~LevelScene();

	void init();
	void update(int deltaTime);
	void render();
	void removeWall();

private:
	TileMap* map;
	Player player1;
	Player player2;
	Coin coin1;
	Coin coin2;
	Chest chest1;
	Chest chest2;
	Button button;
	Wall wall;
	Portal portalIn;
	Portal portalOut;
	InvertedBox box;
	vector<Water*> waterBlocks;
	vector<Water*> inversedWaterBlocks;
	int winCounter;
	bool transfering1, transfering2, remove, firstPress;

	enum Entities {
		PLAYER1, PLAYER2, COIN1, COIN2, CHEST1, CHEST2, BUTTON, WALL, PORTALIN, PORTALOUT, PORTALIN2, PORTALOUT2, INVERTEDBOX
	};
};

