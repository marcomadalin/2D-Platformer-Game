#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"


Coin::~Coin()
{
	sprite->free();
}

enum CoinAnims
{
	APPEARS, DISAPPEARS
};

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	float scale = 0.25f;
	spritesheet.loadFromFile("images/MonedaD.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(APPEARS, 8);
	sprite->addKeyframe(APPEARS, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(APPEARS, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(APPEARS, glm::vec2(2 * scale, 0 * scale));
	sprite->addKeyframe(APPEARS, glm::vec2(3 * scale, 0 * scale));
	sprite->addKeyframe(APPEARS, glm::vec2(0 * scale, 1 * scale));

	sprite->setAnimationSpeed(DISAPPEARS, 1);
	sprite->addKeyframe(DISAPPEARS, glm::vec2(1 * scale, 2 * scale));

	sprite->changeAnimation(APPEARS);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	
}
void Coin::update(int deltaTime, bool collected) 
{
	sprite->update(deltaTime);
	if (collected && sprite->animation() == APPEARS)
		sprite->changeAnimation(DISAPPEARS);

}

void Coin::render()
{
	sprite->render();
}

void Coin::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
