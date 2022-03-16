#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{
public:
	Scene(string fn);
	~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();
	virtual void removeWall();

protected:
	void setTexProgram();
	void initShaders();

protected:
	ShaderProgram texProgram;
	glm::mat4 projection;
	float currentTime;
	string fileName;
	Texture imageTex;
	Sprite* image;

};


#endif // _SCENE_INCLUDE

