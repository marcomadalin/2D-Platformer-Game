#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPIKE 35
#define CACTUS 36
#define CHEST 65
#define COIN 66

#define WATER1 6
#define WATER2 14


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	~Player();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, string texturePath);
	void update(int deltaTime, bool ending, bool transfering1, bool transfering2, glm::vec2 posPortal1, glm::vec2 posPortal2);
	void updateInversed(int deltaTime, bool ending, glm::ivec2 posWall, bool walls, glm::ivec2 posBox);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isInGoal() const { return inGoal; }
	bool isAlive() const { return alive; }
	bool coinCollected() const { return hasCoin; }
	glm::ivec2 getPosition() const { return posPlayer; };
	
private:
	bool collisionWallLeft(glm::ivec2 posWall, bool walls);
	bool collisionWallRight(glm::ivec2 posWall, bool walls);
	bool collisionWallDown(glm::ivec2 posWall, int* posY, bool walls);
	bool collisionWallUp(glm::ivec2 posWall, bool walls);
	bool collisionBoxDown(glm::ivec2 posBox, int* posY);
	bool collisionBoxLeft(glm::ivec2 posBox);
	bool collisionBoxRight(glm::ivec2 posBox);
	bool bJumping, inGoal, alive, hasCoin, first, firstInversed;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int deathCounter;

	enum PlayerAnims
	{
		STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, FALL_LEFT, JUMP_RIGHT, FALL_RIGHT, DIE_LEFT, DIE_RIGHT
	};
};


#endif // _PLAYER_INCLUDE


