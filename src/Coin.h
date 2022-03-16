#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Coin
{
public:
	~Coin();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, bool collected);
	void render();
	void setPosition(const glm::vec2& pos);

private:

	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _COIN_INCLUDE