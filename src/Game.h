#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"
#include "LevelScene.h"
#include "Sound.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}

	static Game &instance()
	{
		static Game G;
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	void updateState();
	bool changeLevel(int key);

private:
	bool bPlay, superUser;			// Continue to play game?
	vector<Scene*> scenes;			// Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that we can have access at any time
	int currentScene, lastLevelScene;
	Sound soundtrack;

	enum scenes
	{
		TITLE, INSTRUCTIONS, LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, LEVEL6, LEVEL7, ENDGAME, CREDITS, LEVELCOMPLETED, GAMEOVER
	};

	friend class LevelScene;
	friend class Player;
};


#endif // _GAME_INCLUDE


