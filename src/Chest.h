#ifndef _CHEST_INCLUDE
#define _CHEST_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Chest
{
public:
	~Chest();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath);
	void update(int deltaTime, bool playerHere);
	void render();
	void setPosition(const glm::vec2& pos);

private:

	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _CHEST_INCLUDE