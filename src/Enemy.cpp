#include "Enemy.h"
#include "Player.h"

namespace {
	const std::string ENEMY_SPRITE_FILE = "content/sprites/enemy1-1.png";
	const float WALK_SPEED = 0.1f;
}

Enemy::Enemy() {
}

Enemy::Enemy(Graphics & graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	Coordinates spawnPoint, int TimeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, (float)spawnPoint.x, (float)spawnPoint.y, TimeToUpdate),
	_facing(RIGHT),
	_maxHealth(0),
	_currentHealth(0)
{}

void Enemy::update(int elapsedTime, Player &player) {
	(void)player;
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics & graphics){
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}