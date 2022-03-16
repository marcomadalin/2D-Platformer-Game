#include "Water.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


Water::~Water()
{
	sprite->free();
}

enum ChestAnims
{
	WAVE1, WAVE2, WAVE3, WAVE4
};

void Water::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath, int waveNumber)
{
	float scale = 0.5f;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(WAVE1, 4);
	sprite->addKeyframe(WAVE1, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(WAVE1, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(WAVE1, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(WAVE1, glm::vec2(1 * scale, 1 * scale));

	sprite->setAnimationSpeed(WAVE2, 4);
	sprite->addKeyframe(WAVE2, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(WAVE2, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(WAVE2, glm::vec2(1 * scale, 1 * scale));
	sprite->addKeyframe(WAVE2, glm::vec2(0 * scale, 0 * scale));

	sprite->setAnimationSpeed(WAVE3, 4);
	sprite->addKeyframe(WAVE3, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(WAVE3, glm::vec2(1 * scale, 1 * scale));
	sprite->addKeyframe(WAVE3, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(WAVE3, glm::vec2(0 * scale, 1 * scale));

	sprite->setAnimationSpeed(WAVE4, 4);
	sprite->addKeyframe(WAVE4, glm::vec2(1 * scale, 1 * scale));
	sprite->addKeyframe(WAVE4, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(WAVE4, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(WAVE4, glm::vec2(1 * scale, 0 * scale));


	sprite->changeAnimation(waveNumber);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void Water::update(int deltaTime) {
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
void Water::render()
{
	sprite->render();
}
void Water::setPosition(const glm::vec2& pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
