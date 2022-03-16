#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }
	glm::ivec2 getInitPos(int entity) const;

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveDownInversed(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUpInversed(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionObject(const glm::ivec2& pos, const glm::ivec2& size, int obj) const;
	bool collisionObjectInversed(const glm::ivec2& pos, const glm::ivec2& size, int obj) const;
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, initPosPlayer1, initPosPlayer2;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	glm::ivec2 entityPos[13];

	enum Entities {
		PLAYER1, PLAYER2, COIN1, COIN2, CHEST1, CHEST2, BUTTON, WALL, PORTALIN, PORTALOUT, PORTALIN2, PORTALOUT2, INVERTEDBOX
	};

};


#endif // _TILE_MAP_INCLUDE
