#include "Player.h"
#include "Graphics.h"
#include "Enemy.h"
#include "Level.h"


namespace {
	const std::string PLAYER_SPRITE_FILE= "content/sprites/player/player-spritesheet.png";
	const int PLAYER_SPRITE_WIDTH = 32;
	const int PLAYER_SPRITE_HEIGHT = 32;
	const float WALK_SPEED = 0.1f;
}

Player::Player() {
}

Player::Player(Graphics &graphics, Coordinates spawnPoint) :
	AnimatedSprite(graphics, PLAYER_SPRITE_FILE, 0, 0, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, (float)spawnPoint.x, (float)spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_maxHealth(100),
	_maxSanity(100),
	_canMove(true),
	_canAttack(true),
	_immunityTimer(0)
{
	graphics.loadImage(PLAYER_SPRITE_FILE);

	this->_currentHealth = this->_maxHealth;
	this->_currentSanity = this->_maxSanity;

	this->setupAnimations();
	this->stopMoving();
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleDown", Coordinates(0, 0));
	this->addAnimation(1, 0, 1, "IdleUp", Coordinates(0, 0));
	this->addAnimation(1, 0, 0, "IdleRight", Coordinates(0, 0));
	this->addAnimation(1, 0, 1, "IdleLeft", Coordinates(0, 0));
	this->addAnimation(1, 0, 0, "RunDown", Coordinates(0, 0));
	this->addAnimation(1, 0, 1, "RunUp", Coordinates(0, 0));
	this->addAnimation(1, 0, 0, "RunRight", Coordinates(0, 0));
	this->addAnimation(1, 0, 1, "RunLeft", Coordinates(0, 0));
	this->addAnimation(1, 1, 0, "AttackDown", Coordinates(0, 0));
	this->addAnimation(1, 1, 1, "AttackUp", Coordinates(0, 0));
	this->addAnimation(1, 1, 0, "AttackRight", Coordinates(0, 0));
	this->addAnimation(1, 1, 1, "AttackLeft", Coordinates(0, 0));
	this->addAnimation(1, 0, 0, "DeathRight", Coordinates(0, 0));
	this->addAnimation(1, 0, 1, "DeathLeft", Coordinates(0, 0));
}

void Player::addAnimation(int frames, int gridX, int gridY, std::string name, Coordinates offsets) {
	AnimatedSprite::addAnimation(frames, gridX * PLAYER_SPRITE_WIDTH, gridY * PLAYER_SPRITE_HEIGHT, name, PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, offsets);
}

void Player::move(int direction) {
	if (! this->_canMove) {
		return;
	}
	switch (direction) {
		case LEFT:
			this->_dx = -WALK_SPEED;
			this->playAnimation("RunLeft");
			this->_facing = LEFT;
			break;
		case RIGHT:
			this->_dx = WALK_SPEED;
			this->playAnimation("RunRight");
			this->_facing = RIGHT;
			break;
		case UP:
			this->_dy = -WALK_SPEED;
			this->playAnimation("RunUp");
			this->_facing = UP;
			break;
		case DOWN:
			this->_dy = WALK_SPEED;
			this->playAnimation("RunDown");
			this->_facing = DOWN;
			break;
		default:
			throw std::range_error("invalid direction");
	}
}

void Player::attack() {
	if (! this->_canAttack) {
		return;
	}
	switch (this->_facing) {
		case LEFT:
			this->playAnimation("AttackLeft");
			break;
		case RIGHT:
			this->playAnimation("AttackRight");
			break;
		case UP:
			this->playAnimation("AttackUp");
			break;
		case DOWN:
			this->playAnimation("AttackDown");
			break;
		default:
			throw std::range_error("invalid direction");
	}
}

void Player::stopMoving() {
	this->_dx = 0;
	this->_dy = 0;
	if (! this->_canMove) {
		return;
	}
	switch (this->_facing) {
		case LEFT:
			this->playAnimation("IdleLeft");
			break;
		case RIGHT:
			this->playAnimation("IdleRight");
			break;
		case UP:
			this->playAnimation("IdleUp");
			break;
		case DOWN:
			this->playAnimation("IdleDown");
			break;
		default:
			throw std::range_error("invalid direction");
	}
}

void Player::death() {
	this->stopMoving();
	this->_canMove = false;
	this->_canAttack = false;
	this->playAnimation("DeathRight", true);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Player::update(int elapsedTime) {
	this->_x += this->_dx * elapsedTime;
	this->_y += this->_dy * elapsedTime;
	this->_immunityTimer = (this->_immunityTimer > 0) ? (this->_immunityTimer - elapsedTime) : 0;
	
	AnimatedSprite::update(elapsedTime);
}

void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	for (unsigned int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = (float)(others.at(i).getBottom() + 1);
				break;
			case sides::BOTTOM:
				this->_y = (float)(others.at(i).getTop() - this->_boundingBox.getHeight() - 1);
				this->_dy = 0;
				break;
			case sides::LEFT:
				this->_x = (float)(others.at(i).getRight() + 1);
				break;
			case sides::RIGHT:
				this->_x = (float)(others.at(i).getLeft() - this->_boundingBox.getWidth() - 1);
				break;
			}
		}
	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics) {
	for (unsigned int i = 0; i < others.size(); i++) {
		level = Level(others.at(i).getDestination(), graphics);
		this->_x = (float)level.getPlayerSpawnPoint().x;
		this->_y = (float)level.getPlayerSpawnPoint().y;
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*> &others) {
	for (unsigned int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
	}
}

void Player::animationDone(std::string currentAnimation) {
}

void Player::setCurrentHealth(int value) {
	this->_currentHealth = value;
}

void Player::setCurrentSanity(int value) {
	this->_currentSanity = value;
}

void Player::gainHealth(int amount) {
	if (amount < 0) {
		if (this->isImmune()) {
			return;
		}
		this->gainImmunity(300);
	}
	this->_currentHealth += amount;
}

void Player::gainSanity(int amount) {
	this->_currentSanity += amount;
}

void Player::gainImmunity(int time) {
	this->_immunityTimer += time;
}

bool Player::isImmune() {
	return this->_immunityTimer > 0;
}
