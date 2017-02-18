#ifndef ENEMY_H
#define ENEMY_H

#include "Graphics.h"
#include "AnimatedSprite.h"
#include "Globals.h"

#include <string>

class Player;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath,int sourceX,int sourceY,
		int width,int height, Coordinates spawnPoint, int TimeToUpdate);

	virtual void draw(Graphics &graphics);
	virtual void update(int elapsedTime, Player &player);
	virtual void touchPlayer(Player* player) = 0;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

protected:
	float _dx, _dy;
	Direction _facing;

	int _maxHealth;
	int _currentHealth;

private:

	float _startingX, _startingY;
	float _shouldMoveUp;

};

#endif