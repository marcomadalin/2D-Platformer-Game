#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


Player::~Player()
{
	sprite->free();
	map->free();
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath)
{
	first = firstInversed = true;
	bJumping = false;
	inGoal = false;
	alive = true;
	hasCoin = false;
	deathCounter = 0;
	float scale = 0.125;
	spritesheet.loadFromFile(texturePath, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(scale, scale), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0 * scale, 0 * scale));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1 * scale, 0 * scale));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0 * scale, 0 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(5 * scale, 2 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4 * scale, 2 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * scale, 2 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * scale, 2 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * scale, 2 * scale));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0 * scale, 2 * scale));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 * scale, 0 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * scale, 1 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 * scale, 1 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * scale, 1 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 * scale, 1 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4 * scale, 1 * scale));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 * scale, 1 * scale));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(7 * scale, 4 * scale));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(6 * scale, 4 * scale));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(5 * scale, 4 * scale));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(4 * scale, 4 * scale));
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(3 * scale, 4 * scale));

	sprite->setAnimationSpeed(FALL_LEFT, 8);
	sprite->addKeyframe(FALL_LEFT, glm::vec2(4 * scale, 4 * scale));
	sprite->addKeyframe(FALL_LEFT, glm::vec2(3 * scale, 4 * scale));
	sprite->addKeyframe(FALL_LEFT, glm::vec2(2 * scale, 4 * scale));
	sprite->addKeyframe(FALL_LEFT, glm::vec2(1 * scale, 4 * scale));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0 * scale, 3 * scale));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(1 * scale, 3 * scale));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(2 * scale, 3 * scale));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(3 * scale, 3 * scale));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(4 * scale, 3 * scale));

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(3 * scale, 3 * scale));
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(4 * scale, 3 * scale));
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(5 * scale, 3 * scale));
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(6 * scale, 3 * scale));

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	sprite->addKeyframe(DIE_LEFT, glm::vec2(7 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(6 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(5 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(4 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(3 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(2 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(1 * scale, 6 * scale));
	sprite->addKeyframe(DIE_LEFT, glm::vec2(0 * scale, 6 * scale));

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(0 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(1 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(2 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(3 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(4 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(5 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(6 * scale, 5 * scale));
	sprite->addKeyframe(DIE_RIGHT, glm::vec2(7 * scale, 5 * scale));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime, bool ending, bool transfering1, bool transfering2, glm::vec2 posPortal1, glm::vec2 posPortal2)
{
	Game& g = Game::instance();
	sprite->update(deltaTime);
	bool movedOnGround = false;

	if (deathCounter == 0 && !ending) {
		if (!g.superUser && map->collisionObject(posPlayer, glm::ivec2(18, 32), WATER1)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
		if (g.getSpecialKey(GLUT_KEY_LEFT) || g.getKey('a') || g.getKey('A'))
		{
			if (bJumping) g.soundtrack.stopWalk();
			else movedOnGround = true;

			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(18, 32)))
			{
				movedOnGround = false;
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (g.getSpecialKey(GLUT_KEY_RIGHT) || g.getKey('d') || g.getKey('D'))
		{
			if (bJumping) g.soundtrack.stopWalk();
			else movedOnGround = true;

			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(18, 32)))
			{
				movedOnGround = false;
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		}

		if (bJumping)
		{
			if (map->collisionMoveUp(posPlayer, glm::ivec2(18, 32), &posPlayer.y)) {
				if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(FALL_RIGHT);
				else if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(FALL_LEFT);
				bJumping = false;
			}
			else {
				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180)
				{
					if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
					else sprite->changeAnimation(STAND_RIGHT);
					bJumping = false;
					posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90)
						if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(FALL_LEFT);
					if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(FALL_RIGHT);
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(18, 32), &posPlayer.y);
					if (!bJumping) {
						if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
						else sprite->changeAnimation(STAND_RIGHT);
					}
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(18, 32), &posPlayer.y))
			{
				if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == FALL_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				if (g.getSpecialKey(GLUT_KEY_UP) || g.getKey(32) || g.getKey('w') || g.getKey('W'))
				{
					g.soundtrack.playJump();
					if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
					else sprite->changeAnimation(JUMP_LEFT);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}

		if (!movedOnGround)g.soundtrack.stopWalk();
		else g.soundtrack.playWalk();

		if (map->collisionObject(posPlayer, glm::ivec2(18, 32), COIN)) {
			hasCoin = true;
			if (first) {
				g.soundtrack.stopWalk();
				g.soundtrack.playCoin();
				g.soundtrack.playWalk();
				first = false;
			}
		}
		if (map->collisionObject(posPlayer, glm::ivec2(18, 32), CHEST) && hasCoin) inGoal = true;
		else inGoal = false;

		if (!g.superUser && map->collisionObject(posPlayer, glm::ivec2(18, 32), SPIKE)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
		if (!g.superUser && map->collisionObject(posPlayer, glm::ivec2(18, 32), CACTUS)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);

		}
		if (transfering1)
			setPosition(posPortal2);
		if (transfering2) setPosition(posPortal1);
	}
	else {
		if (deathCounter >= 40) alive = false;
		else ++deathCounter;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}
void Player::updateInversed(int deltaTime, bool ending, glm::ivec2 posWall, bool walls, glm::ivec2 posBox)
{
	Game& g = Game::instance();
	sprite->update(deltaTime);

	if (deathCounter == 0 && !ending) {
		if (!g.superUser && map->collisionObjectInversed(posPlayer, glm::ivec2(18, 26), WATER2)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);
		}
		else if (g.getSpecialKey(GLUT_KEY_LEFT) || g.getKey('a') || g.getKey('A'))
		{
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);

			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(18, 26)) || collisionWallLeft(posWall, walls) || collisionBoxLeft(posBox))
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (g.getSpecialKey(GLUT_KEY_RIGHT) || g.getKey('d') || g.getKey('D'))
		{
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(18, 26)) || collisionWallRight(posWall, walls) || collisionBoxRight(posBox))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
		}

		if (bJumping)
		{
			if (map->collisionMoveUpInversed(posPlayer, glm::ivec2(18, 32), &posPlayer.y) || collisionWallUp(posWall, walls)) {
				if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(FALL_RIGHT);
				else if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(FALL_LEFT);
				bJumping = false;
			}
			else {


				jumpAngle += JUMP_ANGLE_STEP;
				if (jumpAngle == 180)
				{
					if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
					else sprite->changeAnimation(STAND_RIGHT);
					bJumping = false;
					posPlayer.y = startY;
				}
				else
				{
					posPlayer.y = int(startY + 96 * sin(3.14159f * jumpAngle / 180.f));
					if (jumpAngle > 90) {
						if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(FALL_LEFT);
						if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(FALL_RIGHT);
						bJumping = !(map->collisionMoveDownInversed(posPlayer, glm::ivec2(18, 26), &posPlayer.y) || collisionWallDown(posWall, &posPlayer.y, walls) || collisionBoxDown(posBox, &posPlayer.y));
						if (!bJumping) {
							if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
							else sprite->changeAnimation(STAND_RIGHT);
						}
					}
				}
			}
		}
		else
		{
			if (!(map->collisionMoveDownInversed(posPlayer, glm::ivec2(18, 26), &posPlayer.y) || collisionWallDown(posWall, &posPlayer.y, walls) || collisionBoxDown(posBox, &posPlayer.y)))
				posPlayer.y -= FALL_STEP;
			else
			{
				if (sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == FALL_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				if (g.getSpecialKey(GLUT_KEY_UP) || g.getKey(32) || g.getKey('w') || g.getKey('W'))
				{
					if (sprite->animation() == STAND_RIGHT || sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(JUMP_RIGHT);
					else sprite->changeAnimation(JUMP_LEFT);
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}

		if (map->collisionObjectInversed(posPlayer, glm::ivec2(18, 26), COIN)) {
			hasCoin = true;
			if (firstInversed) {
				g.soundtrack.stopWalk();
				g.soundtrack.playCoin();
				g.soundtrack.playWalk();
				firstInversed = false;
			}
		}
		if (map->collisionObjectInversed(posPlayer, glm::ivec2(18, 26), CHEST) && hasCoin) inGoal = true;
		else inGoal = false;

		if (!g.superUser && map->collisionObjectInversed(posPlayer, glm::ivec2(18, 32), SPIKE)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);
		}
		if (!g.superUser && map->collisionObjectInversed(posPlayer, glm::ivec2(18, 32), CACTUS)) {
			++deathCounter;
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(DIE_RIGHT);
			else sprite->changeAnimation(DIE_LEFT);
		}
	}
	else {
		if (deathCounter >= 40) alive = false;
		else ++deathCounter;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::collisionWallLeft(glm::ivec2 posWall, bool walls)
{
	int x0 = posWall.x;
	int xf = posWall.x + 34;
	int y0 = posWall.y;
	int yf = posWall.y + 30;

	return ((posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf && posPlayer.y <= yf && posPlayer.y >= y0 && walls);
}
bool Player::collisionWallRight(glm::ivec2 posWall, bool walls)
{
	int x0 = posWall.x;
	int xf = posWall.x + 34;
	int y0 = posWall.y;
	int yf = posWall.y + 30;
	return ((posPlayer.x + 18) >= x0 && (posPlayer.x + 18) <= xf && posPlayer.y <= yf && posPlayer.y >= y0 && walls);
}
bool Player::collisionWallDown(glm::ivec2 posWall, int* posY, bool walls)
{
	int x0, xf, y0, yf;

	x0 = posWall.x;
	xf = posWall.x + 32;
	y0 = posWall.y;
	yf = posWall.y + 32;

	if ((((posPlayer.x + 18) >= x0 && (posPlayer.x + 18) <= xf) ||
		((posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf)) &&
		posPlayer.y + 1 <= yf && posPlayer.y + 1 >= y0 && walls)
	{
		*posY = yf - 1;
		return true;
	}
	return false;
}
bool Player::collisionWallUp(glm::ivec2 posWall, bool walls)
{
	int x0, xf, y0, yf;

	x0 = posWall.x + 2;
	xf = posWall.x + 32;
	y0 = posWall.y;
	yf = posWall.y + 32;

	return ((((posPlayer.x + 18) >= x0 && (posPlayer.x + 18) <= xf) ||
		((posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf)) &&
		posPlayer.y + 32 <= yf && posPlayer.y + 32 >= y0 && walls);
}

bool Player::collisionBoxDown(glm::ivec2 posBox, int* posY)
{
	int x0, xf, y0, yf;

	x0 = posBox.x;
	xf = posBox.x + 16;
	y0 = posBox.y;
	yf = posBox.y + 20;

	if (((posPlayer.x + 18) >= x0 && (posPlayer.x + 18) <= xf) ||
		((posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf)) {
		if (posPlayer.y + 1 <= yf && posPlayer.y + 1 >= y0)
		{
			*posY = yf - 1;
			return true;
		}
	}
	return false;
}
bool Player::collisionBoxRight(glm::ivec2 posBox) {
	int x0 = posBox.x + 3;
	int xf = posBox.x + 16;
	int y0 = posBox.y;
	int yf = posBox.y + 18;
	return ((posPlayer.x + 18) >= x0 && (posPlayer.x + 18) <= xf && (posPlayer.y + 1) <= yf && (posPlayer.y + 1) >= y0);
}
bool Player::collisionBoxLeft(glm::ivec2 posBox) {
	int x0 = posBox.x;
	int xf = posBox.x + 13;
	int y0 = posBox.y;
	int yf = posBox.y + 18;

	return ((posPlayer.x + 4) >= x0 && (posPlayer.x + 4) <= xf && (posPlayer.y + 1) <= yf && (posPlayer.y + 1) >= y0);
}


