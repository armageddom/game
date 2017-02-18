#include "Game.h"

#include <algorithm>

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	Graphics graphics;
	Input input;
	this->_graphics = graphics;
	this->_input = input;

	this->_level = Level("map1", graphics);
	this->_player = Player(graphics, _level.getPlayerSpawnPoint());
	this->_hud = HUD(graphics);

	this->gameLoop();
}

Game::~Game() {
}

void Game::gameLoop() {
	SDL_Event event;
	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true) {
		this->_input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					this->_input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				this->_input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		this->handleInput();

		if (this->_player.getCurrentHealth() <= 0) {
			this->_player.death();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw();
	}
}

void Game::handleInput() {
	if (this->_input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
		this->_player.move(LEFT);
	}
	if (this->_input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
		this->_player.move(RIGHT);
	}
	if (this->_input.isKeyHeld(SDL_SCANCODE_UP) == true) {
		this->_player.move(UP);
	}
	if (this->_input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
		this->_player.move(DOWN);
	}
	if (this->_input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
		this->_player.attack();
	}
	if (! this->_input.isKeyHeld(SDL_SCANCODE_LEFT) && 
		! this->_input.isKeyHeld(SDL_SCANCODE_RIGHT) && 
		! this->_input.isKeyHeld(SDL_SCANCODE_UP) &&
		! this->_input.isKeyHeld(SDL_SCANCODE_DOWN) &&
		! this->_input.isKeyHeld(SDL_SCANCODE_SPACE))
	{
			this->_player.stopMoving();
	}
}

void Game::update(int elapsedTime) {
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);
	this->_graphics.setViewOffset(Coordinates((int)(Globals::SCREEN_CENTER_X - this->_player.getX()), (int)(Globals::SCREEN_CENTER_Y - this->_player.getY())));

	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleTileCollisions(others);
	}
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollision(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleEnemyCollisions(otherEnemies);
	}
	std::vector<Door> otherDoors;
	if ((otherDoors = this->_level.checkDoorCollision(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics);
	}
}

void Game::draw() {
	this->_graphics.clear();

	this->_level.drawBackground(this->_graphics);
	this->_level.drawEnvironment(this->_graphics);
	this->_level.drawObjects(this->_graphics);
	this->_player.draw(this->_graphics);
	this->_level.drawForeground(this->_graphics);
	this->_hud.draw(this->_graphics);

	this->_graphics.flip();
}