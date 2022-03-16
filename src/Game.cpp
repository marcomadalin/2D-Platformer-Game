#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init()
{
	currentScene = TITLE;
	lastLevelScene = LEVEL1;
	bPlay = true;
	superUser = false;
	glClearColor(0.9f, 0.9f, 0.7f, 1.0f);
	scenes = vector<Scene*>(13);

	scenes[0] = new Scene("title.png");
	scenes[1] = new Scene("instructions.png");
	scenes[2] = new LevelScene("level01.txt");
	scenes[3] = new LevelScene("level02.txt");
	scenes[4] = new LevelScene("level03.txt");
	scenes[5] = new LevelScene("level04.txt");
	scenes[6] = new LevelScene("level05.txt");
	scenes[7] = new LevelScene("level06.txt");
	scenes[8] = new LevelScene("level07.txt");
	scenes[9] = new Scene("endGame.png");
	scenes[10] = new Scene("credits.png");
	scenes[11] = new Scene("levelCompleted.png");
	scenes[12] = new Scene("gameOver.png");

	soundtrack.init();
	soundtrack.playMusic();

	for (Scene* s : scenes) s->init();
}

bool Game::update(int deltaTime)
{
	scenes[currentScene]->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scenes[currentScene]->render();
}

void Game::keyPressed(int key)
{
	keys[key] = true;
	if(key == 27) bPlay = false;	// Escape code
	else if (key == 'R' || key == 'r') scenes[currentScene]->init();
	else if (!(superUser && changeLevel(key))) updateState();
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
	if (key == GLUT_KEY_F1) superUser = !superUser;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
	updateState();
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::updateState()
{
	switch (currentScene)
	{
	case TITLE:
		++currentScene;
		soundtrack.playKeyPressed();
		break;
	case INSTRUCTIONS:
		++currentScene;
		soundtrack.playKeyPressed();
		break;
	case ENDGAME:
		++currentScene;
		break;
	case CREDITS:
		bPlay = false;
		break;
	case LEVELCOMPLETED:
		currentScene = lastLevelScene + 1;
		break;
	case GAMEOVER:
		if (getKey(13)) {
 			currentScene = lastLevelScene;
			scenes[currentScene]->init();
			soundtrack.playKeyPressed();
			soundtrack.playMusic();
		}
		break;
	}
}

bool Game::changeLevel(int key)
{
	if (key >= '1' && key <= '7') {
		currentScene = LEVEL1 + int(key - '1');
		lastLevelScene = currentScene;
		scenes[currentScene]->init();
		return true;
	}
	else if (key == 'c' || key == 'C') {
		scenes[currentScene]->removeWall();
		return true;
	}
	return false;
}



