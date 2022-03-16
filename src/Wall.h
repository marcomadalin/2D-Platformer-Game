#ifndef _WALL_INCLUDE
#define _WALL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Wall
{
public:
	~Wall();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath);
	void update(int deltaTime, bool disappear);
	void render();
	void setPosition(const glm::vec2& pos);

private:
	bool collisionPlayer(glm::ivec2 posPlayer);
	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _WALL_INCLUDE