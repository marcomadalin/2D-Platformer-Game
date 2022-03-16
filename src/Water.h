#ifndef _WATER_INCLUDE
#define _WATER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Water
{
public:
	~Water();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath, int waveNumber);
	void update(int deltatime);
	void render();
	void setPosition(const glm::vec2& pos);

private:

	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _WATER_INCLUDE