#include "InvertedBox.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

InvertedBox::~InvertedBox()
{
	sprite->free();
	map->free();
}
void InvertedBox::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath)
{
	float scale = 1;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 20), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0 * scale, 0 * scale));
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void InvertedBox::update(int deltaTime, glm::vec2 posPlayer, bool walls, glm::vec2 posWall)
{

	sprite->update(deltaTime);

	if (pushedLeft(posPlayer)) {
		position.x -= 2;
		if (map->collisionMoveLeft(position, glm::ivec2(16, 16)) || collisionWallLeft(posWall, walls)) 
			position.x += 2;
	}
	else if (pushedRight(posPlayer)) {
		position.x += 2;
		if (map->collisionMoveRight(position, glm::ivec2(16, 16)) || collisionWallRight(posWall, walls)) 
			position.x -= 2;
	}

	if (!(map->collisionMoveDownInversed(position, glm::ivec2(16, 16), &position.y) || collisionWallDown(posWall, &position.y, walls)))
		position.y -= FALL_STEP;




	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}
void InvertedBox::render()
{
	sprite->render();
}
void InvertedBox::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}
void InvertedBox::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
bool InvertedBox::collisionWallLeft(glm::ivec2 posWall, bool walls)
{
	int x0 = posWall.x;
	int xf = posWall.x + 31;
	int y0 = posWall.y;
	int yf = posWall.y + 32;

	return (position.x >= x0 && position.x <= xf && position.y <= yf && position.y >= y0 && walls);
}
bool InvertedBox::collisionWallRight(glm::ivec2 posWall, bool walls)
{
	int x0 = posWall.x +1;
	int xf = posWall.x + 32;
	int y0 = posWall.y;
	int yf = posWall.y + 32;
	return ((position.x + 16) >= x0 && (position.x + 16) <= xf && position.y <= yf && position.y >= y0 && walls);

}
bool InvertedBox::collisionWallDown(glm::ivec2 posWall, int* posY, bool walls)
{
	int x0, xf, y0, yf;

	x0 = posWall.x;
	xf = posWall.x + 32;
	y0 = posWall.y;
	yf = posWall.y + 32;

	if ((((position.x + 16) >= x0 && (position.x + 20) <= xf) ||
		(position.x>= x0 && position.x <= xf)) &&
		position.y <= yf && position.y >= y0 && walls)
	{
		*posY = yf;
		return true;
	}
	return false;

}
bool InvertedBox::pushedLeft(glm::ivec2 posPlayer) 
{
	int x0 = position.x;
	int xf = position.x + 16;
	int y0 = position.y;
	int yf = position.y + 10 ;

	return ((posPlayer.x + 4) > x0 && (posPlayer.x + 4) < xf && (posPlayer.y + 1) <= yf && (posPlayer.y + 1) >= y0);
}
bool InvertedBox::pushedRight(glm::ivec2 posPlayer)
{
	int x0 = position.x;
	int xf = position.x + 16;
	int y0 = position.y;
	int yf = position.y + 10;

	return ((posPlayer.x + 18) > x0 && (posPlayer.x + 18) < xf && (posPlayer.y + 1) <= yf && (posPlayer.y + 1) >= y0);
}


