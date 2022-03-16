#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Wall.h"

Wall::~Wall()
{
	sprite->free();
}

enum WallAnims
{
	STAND, DISAPPEAR
};

void Wall::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath)
{
	float scale = 0.5f;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0 * scale, 0 * scale));


	sprite->setAnimationSpeed(DISAPPEAR, 1);
	sprite->addKeyframe(DISAPPEAR, glm::vec2(1 * scale, 0 * scale));


	sprite->changeAnimation(STAND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void Wall::update(int deltaTime, bool disappear)
{
	sprite->update(deltaTime);
	if (sprite->animation() == STAND && disappear)
		sprite->changeAnimation(DISAPPEAR);
}
void Wall::render()
{
	sprite->render();
}
void Wall::setPosition(const glm::vec2& pos)
{

	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}


