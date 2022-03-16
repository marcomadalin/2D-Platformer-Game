#include "Sound.h"

Sound::Sound() {
	engine = nullptr;
	levelPassed = gameOver = keyPressed = jump = coin = button = portal = nullptr;
	music = walk = nullptr;
}

Sound::~Sound() {
	if (engine != nullptr) {
		engine->drop();
	}
}

void Sound::init() {
	engine = irrklang::createIrrKlangDevice();

	music = engine->play2D("sounds\\background.mp3", true, true);
	music->setVolume(0.3f);
	walk = engine->play2D("sounds\\walk.wav", true, true, true);
	walk->setVolume(0.7f);

	coin = engine->addSoundSourceFromFile("sounds\\coin.wav");
	coin->setDefaultVolume(0.3);
	portal = engine->addSoundSourceFromFile("sounds\\portal.wav");
	portal->setDefaultVolume(0.9);
	button = engine->addSoundSourceFromFile("sounds\\button.ogg");
	button->setDefaultVolume(0.3);
	levelPassed = engine->addSoundSourceFromFile("sounds\\levelPassed.ogg");
	levelPassed->setDefaultVolume(0.5);
	gameOver = engine->addSoundSourceFromFile("sounds\\gameOver.wav");
	gameOver->setDefaultVolume(0.5);
	jump = engine->addSoundSourceFromFile("sounds\\jump.wav");
	jump->setDefaultVolume(0.025f);
	keyPressed = engine->addSoundSourceFromFile("sounds\\keyPressed.ogg");
	keyPressed->setDefaultVolume(0.45f);
}

void Sound::playMusic()
{
	music->setIsPaused(false);
}

void Sound::stopMusic()
{
	music->setIsPaused(true);
}

void Sound::playWalk()
{
	walk->setIsPaused(false);
}

void Sound::stopWalk()
{
	walk->setIsPaused(true);
}

void Sound::playJump()
{
	engine->play2D(jump);
}

void Sound::playKeyPressed()
{
	engine->play2D(keyPressed);
}

void Sound::playButton()
{
	engine->play2D(button);
}

void Sound::playPortal()
{
	engine->play2D(portal);
}

void Sound::playCoin()
{
	engine->play2D(coin);
}

void Sound::playLevelPassed()
{
	engine->play2D(levelPassed);
}

void Sound::playGameOver()
{
	engine->play2D(gameOver);
}
