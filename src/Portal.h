#ifndef _PORTAL_INCLUDE
#define _PORTAL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Portal
{
public:
	~Portal();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath, int iniAnimation);
	bool update(int deltaTime, glm::ivec2 posPlayer, bool transfered);
	void render();
	void setPosition(const glm::vec2& pos);
	bool playerInPortal(glm::ivec2 posPlayer);

private:
	int transfering, waitingTime, waitingTime2;
	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 position, tileMapDispl;
};

#endif // _PORTAL_INCLUDE