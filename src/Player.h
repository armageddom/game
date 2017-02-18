#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h"
#include "AnimatedSprite.h"
#include "Door.h"

class Level;
class Graphics;
class Enemy;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Coordinates spawnPoint);
	void draw(Graphics &graphics);

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
	void handleEnemyCollisions(std::vector<Enemy*> &others);

	void update(int elapsedTime);

	void move(int direction);
	void attack();
	void stopMoving();
	void death();

	void addAnimation(int frames, int gridX, int gridY, std::string name, Coordinates offsets);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const inline int getMaxHealth() const { return this->_maxHealth; };
	const inline int getCurrentHealth() const { return this->_currentHealth; };

	const inline int getMaxSanity() const { return this->_maxSanity; };
	const inline int getCurrentSanity() const { return this->_currentSanity; };

	void setCurrentHealth(int value);
	void setCurrentSanity(int value);

	void gainHealth(int amount);
	void gainSanity(int amount);
	void gainImmunity(int time);
	bool isImmune();
private:
	float _dx, _dy;

	int _maxHealth;
	int _currentHealth;

	int _maxSanity;
	int _currentSanity;

	int _immunityTimer;

	bool _canMove;
	bool _canAttack;

	Direction _facing;
};

#endif