#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Portal.h"

Portal::~Portal()
{
	sprite->free();
}

enum PortalAnims
{
	STAND, OPEN, CLOSED
};

void Portal::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath, int iniAnimation)
{
	transfering = waitingTime = waitingTime2= 0;
	float scale = 0.125f;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(STAND, 8);
	sprite->addKeyframe(STAND, glm::vec2(0 * scale, 5 * scale));
	sprite->addKeyframe(STAND, glm::vec2(1 * scale, 5 * scale));
	sprite->addKeyframe(STAND, glm::vec2(2 * scale, 5 * scale));
	sprite->addKeyframe(STAND, glm::vec2(3 * scale, 5 * scale));
	sprite->addKeyframe(STAND, glm::vec2(4 * scale, 5 * scale));


	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(2 * scale, 0 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(3 * scale, 0 * scale));

	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 1 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(2 * scale, 1 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(3 * scale, 1 * scale));

	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 2 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 2 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(2 * scale, 2 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(3 * scale, 2 * scale));

	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 3 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(1 * scale, 3 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(2 * scale, 3 * scale));
	sprite->addKeyframe(OPEN, glm::vec2(3 * scale, 3 * scale));

	sprite->addKeyframe(OPEN, glm::vec2(0 * scale, 4 * scale));

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0 * scale, 4 * scale));

	sprite->changeAnimation(iniAnimation);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
bool Portal::update(int deltaTime, glm::ivec2 posPlayer, bool transfered)
{
	sprite->update(deltaTime);

	if (playerInPortal(posPlayer) && waitingTime2 == 0) {
		if (sprite->animation() == STAND) {
			sprite->changeAnimation(OPEN);
			++transfering;
		}
	}
	if (transfering != 0) ++transfering;
	if (transfering == 30) {
		transfering = 0;
		if (playerInPortal(posPlayer)) {
			sprite->changeAnimation(CLOSED);
			return true;
		}
		else {
			sprite->changeAnimation(STAND);
			return false;
		}
	}

	if (transfered) {
		if (sprite->animation()==CLOSED)sprite->changeAnimation(OPEN);
		++waitingTime;
	}
	if (waitingTime != 0) ++waitingTime;
	if (waitingTime2 != 0) ++waitingTime2;
	if (waitingTime == 30) {
		waitingTime = 0;
		sprite->changeAnimation(STAND);
		++waitingTime2;
	}
	if (waitingTime2 == 60) waitingTime2 = 0;
	return false;
}
void Portal::render()
{
	sprite->render();
}
void Portal::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

bool Portal::playerInPortal(glm::ivec2 posPlayer) 
{
	int x0, xf, y0, yf;
	x0 = position.x + 5;
	xf = position.x + 27;
	y0 = position.y;
	yf = position.y + 32;

	return  (posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf &&
			posPlayer.y >= y0 && posPlayer.y <= yf;
}