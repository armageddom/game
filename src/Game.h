#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "Graphics.h"
#include "Player.h"
#include "Level.h"
#include "HUD.h"
#include "Enemy.h"
#include "Bar.h"
#include "Input.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw();
	void update(int elapsedTime);
	void inline handleInput();

	Player _player;
	Level _level;
	HUD _hud;

	Graphics _graphics;
	Input _input;
};

#endif
