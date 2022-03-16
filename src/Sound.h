#pragma once
#include <irrKlang.h>
#include <string>

class Sound
{
public:
	Sound();
	~Sound();

	void init();
	void playMusic();
	void stopMusic();
	void playWalk();
	void stopWalk();
	void playJump();
	void playKeyPressed();
	void playButton();
	void playPortal();
	void playCoin();
	void playLevelPassed();
	void playGameOver();

private:
	irrklang::ISoundEngine* engine;
	irrklang::ISoundSource* jump, * coin, * button, * portal;
	irrklang::ISoundSource* levelPassed, * gameOver, * keyPressed;
	irrklang::ISound* music, * walk;
};
