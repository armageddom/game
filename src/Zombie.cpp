#include "Zombie.h"

namespace {
	const std::string ZOMBIE_SPRITE_FILE = "content/sprites/enemies/enemy1-1.png";

}
Zombie::Zombie(){}

Zombie::Zombie(Graphics & graphics, Coordinates spawnPoint) :
	Enemy(graphics, ZOMBIE_SPRITE_FILE, 1, 1, 16, 16, spawnPoint, 140),
	_startingX((float)spawnPoint.x),
	_startingY((float)spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimations();
	this->playAnimation("Move");
}

void Zombie::update(int elapsedTime, Player &player) {
	this->playAnimation("Move");
	this->_y += this->_shouldMoveUp ? -.08f : .08f;
	if (this->_y > (this->_startingY + 40) || this->_y < this->_startingY - 40) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}
	Enemy::update(elapsedTime, player);
}

void Zombie::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Zombie::touchPlayer(Player* player) {
	player->gainHealth(-2);
}

void Zombie::animationDone(std::string currentAnimation) {
}

void Zombie::setupAnimations() {
	this->addAnimation(3, 0, 0, "Move", 16, 16, Coordinates(0, 0));
	this->addAnimation(1, 16, 0, "Shoot", 16, 16, Coordinates(0, 0));
}