#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

#define SPIKE 35
#define CACTUS 36
#define CHEST 65
#define COIN 66

#define WATER1 6
#define WATER2 14


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	for (int i = 0; i < 13; i++) {
		entityPos[i].x = -1;
		entityPos[i].y = -1;
	}


	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == '0')
				map[j * mapSize.x + i] = 0;
			else if (tile == '?') { //spawnpoint
				map[j * mapSize.x + i] = 0;
				entityPos[PLAYER1].x = i;
				entityPos[PLAYER1].y = j - 1;
			}
			else if (tile == '!') { //spawnpoint
				map[j * mapSize.x + i] = 0;
				entityPos[PLAYER2].x = i;
				entityPos[PLAYER2].y = j - 1;
			}
			else if (tile == '.') { //coin
				map[j * mapSize.x + i] = COIN;
				entityPos[COIN1].x = i;
				entityPos[COIN1].y = j;
			}
			else if (tile == ',') { //coin
				map[j * mapSize.x + i] = COIN;
				entityPos[COIN2].x = i;
				entityPos[COIN2].y = j;
			}
			else if (tile == '_') {
				map[j * mapSize.x + i] = CHEST;
				entityPos[CHEST1].x = i;
				entityPos[CHEST1].y = j;
			}
			else if (tile == '-') {
				map[j * mapSize.x + i] = CHEST;
				entityPos[CHEST2].x = i;
				entityPos[CHEST2].y = j;
			}
			else if (tile == '*') {
				map[j * mapSize.x + i] = 0;
				entityPos[BUTTON].x = i;
				entityPos[BUTTON].y = j;
			}
			else if (tile == '=') {
				map[j * mapSize.x + i] = 0;
				entityPos[WALL].x = i;
				entityPos[WALL].y = j;
			}
			else if (tile == ';') {
				map[j * mapSize.x + i] = 0;
				entityPos[PORTALIN].x = i;
				entityPos[PORTALIN].y = j;
			}
			else if (tile == ':') {
				map[j * mapSize.x + i] = 0;
				entityPos[PORTALOUT].x = i;
				entityPos[PORTALOUT].y = j;
			}
			else if (tile == '&') {
				map[j * mapSize.x + i] = 0;
				entityPos[INVERTEDBOX].x = i;
				entityPos[INVERTEDBOX].y = j;
			}
			else if (tile == '^') map[j * mapSize.x + i] = SPIKE;
			else if (tile == '<') map[j * mapSize.x + i] = CACTUS;
			else if (tile > '0' && tile <= '9')  map[j * mapSize.x + i] = tile - int('0');
			else map[j * mapSize.x + i] = tile - int('A') + 10;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();



	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile > 0 && tile < 65)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % 8) / tilesheetSize.x, float((tile - 1) / 8) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] > 0 && map[y * mapSize.x + x] < SPIKE && map[y * mapSize.x + x] != WATER2) return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] > 0 && map[y * mapSize.x + x] < SPIKE && map[y * mapSize.x + x] != WATER2) return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] > 0 && map[y * mapSize.x + x] < SPIKE)
		{
			if (*posY - tileSize * y + size.y <= 5)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}

	}

	return false;
}

bool TileMap::collisionMoveDownInversed(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		if (map[(y - 1) * mapSize.x + x] > 0 && map[(y - 1) * mapSize.x + x] < SPIKE)
		{
			if (tileSize * y - *posY <= 4)
			{
				*posY = tileSize * y;
				return true;
			}
		}
	}

	return false;
}


bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] > 0 && map[y * mapSize.x + x] < SPIKE) return true;
	}

	return false;
}

bool TileMap::collisionMoveUpInversed(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] > 0 && map[y * mapSize.x + x] < SPIKE) return true;
	}

	return false;
}


bool TileMap::collisionObject(const glm::ivec2& pos, const glm::ivec2& size, int obj)const {
	int x0, x1, y, x, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	if (obj == WATER1) {
		for (int x = x0; x <= x1; x++)
			if (map[(y + 1) * mapSize.x + x] == obj) return true;
	}
	else 
		for (int x = x0; x <= x1; x++)
			if (map[y * mapSize.x + x] == obj) return true;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == obj) return true;
	}

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == obj) return true;
	}

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == obj) return true;
	}

	return false;
}

bool TileMap::collisionObjectInversed(const glm::ivec2& pos, const glm::ivec2& size, int obj)const {
	int x0, x1, y, y0, y1, x;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;


	if (obj == WATER2) {
		for (int x = x0; x <= x1; x++)
			if (map[(y - 1) * mapSize.x + x] == obj) return true;
	}
	else
		for (int x = x0; x <= x1; x++)
			if (map[y * mapSize.x + x] == obj) return true;
	

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
		if (map[y * mapSize.x + x] == obj) return true;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
		if (map[y * mapSize.x + x] == obj)return true;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
		if (map[y * mapSize.x + x] == obj) return true;

	return false;
}

glm::ivec2 TileMap::getInitPos(int entity)const
{
	return entityPos[entity];
}

