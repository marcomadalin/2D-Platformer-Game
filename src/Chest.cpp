#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Chest.h"

Chest::~Chest() 
{
	sprite->free();
}

enum ChestAnims
{
	STAND, OPEN
};

void Chest::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath)
{
	float scale = 0.5f;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0 * scale, 0 * scale));


	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 1 * scale));

	sprite->changeAnimation(STAND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void Chest::update(int deltaTime, bool playerHere)
{
	sprite->update(deltaTime);
	if (playerHere && sprite->animation() == STAND)
		sprite->changeAnimation(OPEN);

}
void Chest::render()
{
	sprite->render();
}
void Chest::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
