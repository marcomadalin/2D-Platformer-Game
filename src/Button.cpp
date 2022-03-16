#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Button.h"

Button::~Button()
{
	sprite->free();
}

enum ButtonAnims
{
	STAND, PRESSED
};

void Button::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath)
{
	float scale = 0.25f;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(STAND, 1);
	sprite->addKeyframe(STAND, glm::vec2(0 * scale, 2 * scale));


	sprite->setAnimationSpeed(PRESSED, 1);
	sprite->addKeyframe(PRESSED, glm::vec2(1 * scale, 2 * scale));


	sprite->changeAnimation(STAND);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
bool Button::update(int deltaTime, glm::ivec2 posPlayer)
{
	sprite->update(deltaTime);
	if (sprite->animation() == STAND && collisionPlayer(posPlayer)) 
		sprite->changeAnimation(PRESSED);
	return sprite->animation() == PRESSED;
}
void Button::render()
{
	sprite->render();
}
void Button::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

bool Button::collisionPlayer(glm::ivec2 posPlayer)
{
	int x0 = position.x;
	int x1 = position.x + 16;
	int y0 = position.y - 16;
	int y1 = position.y - 8;

	return (posPlayer.x + 4 >= x0 && posPlayer.x + 4 < x1 || posPlayer.x + 18 >= x0 && posPlayer.x + 18 < x1) && (posPlayer.y >= y0 && posPlayer.y < y1);
}


