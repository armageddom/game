#include "Sprite.h"
#include "Globals.h"
#include "Exception.h"

Sprite::Sprite() {
}

Sprite::Sprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, float posX, float posY) : _x(posX), _y(posY) {
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filepath));
	if (this->_spriteSheet == NULL) {
		throw Exception::FileNotFound(filepath);
	}
	this->_boundingBox = Rectangle((int)this->_x, (int)this->_y, (int)(width * Globals::SPRITE_SCALE), (int)(height * Globals::SPRITE_SCALE));
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics &graphics, float x, float y, bool ignoreOffset) {
	SDL_Rect destRect= { 
		(int)x,
		(int)y,
		(int)(this->_sourceRect.w * Globals::SPRITE_SCALE),
		(int)(this->_sourceRect.h * Globals::SPRITE_SCALE)
	};
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destRect, ignoreOffset);
}

void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}
void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}
const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}
	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::update() {
	this->_boundingBox = Rectangle((int)this->_x, (int)this->_y, 
		(int)(this->_sourceRect.w * Globals::SPRITE_SCALE),
		(int)(this->_sourceRect.h * Globals::SPRITE_SCALE));
}