#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Button
{
public:
	~Button();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath);
	bool update(int deltaTime, glm::ivec2 posPlayer);
	void render();
	void setPosition(const glm::vec2& pos);

private:
	bool collisionPlayer(glm::ivec2 posPlayer);
	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _BUTTON_INCLUDE