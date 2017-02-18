#pragma once

#include "Enemy.h"
#include "Graphics.h"
#include "AnimatedSprite.h"
#include "Globals.h"
#include "Player.h"
#include <string>

class Zombie : public Enemy {
public:
	Zombie();
	Zombie(Graphics &graphics, Coordinates spawnPoint);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
	bool _shouldMoveLeft;


};
