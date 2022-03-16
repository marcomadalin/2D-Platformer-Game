#ifndef _INVERTEDBOX_INCLUDE
#define _INVERTEDBOX_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

#define FALL_STEP 4

class InvertedBox
{
public:
	~InvertedBox();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath);
	void update(int deltaTime, glm::vec2 posPlayer, bool walls, glm::vec2 posWall);
	void render();
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPosition() const { return position; };

private:

	bool collisionWallLeft(glm::ivec2 posWall, bool walls);
	bool collisionWallRight(glm::ivec2 posWall, bool walls);
	bool collisionWallDown(glm::ivec2 posWall, int* posY, bool walls);
	bool pushedRight(glm::ivec2 posPlayer);
	bool pushedLeft(glm::ivec2 posPlayer);
	glm::ivec2 tileMapDispl, position;
	int startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif // _INVERTEDBOX_INCLUDE

