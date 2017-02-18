#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Graphics.h"

AnimatedSprite::AnimatedSprite() {
}

AnimatedSprite::AnimatedSprite(Graphics &graphics,
	const std::string &filePath,
	int sourceX, int sourceY,
	int width, int height,
	float posX, float posY, int timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0),
	_timeToUpdate(timeToUpdate),
	_timeElapsed(0),
	_visible(true),
	_currentAnimationOnce(false),
	_currentAnimations("")
{
}

void AnimatedSprite::update(int elapsedTime) {
	Sprite::update();

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimations].size() - 1) {
			this->_frameIndex++;
		} else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->_frameIndex = 0;
			this->animationDone(this->_currentAnimations);
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, float x, float y) {
	if (this->_visible) {
		SDL_Rect destinationRectangle;
		destinationRectangle.x = (int)x + this->_offsets[this->_currentAnimations].x;
		destinationRectangle.y = (int)y + this->_offsets[this->_currentAnimations].y;
		destinationRectangle.w = (int)(this->_sourceRect.w * Globals::SPRITE_SCALE);
		destinationRectangle.h = (int)(this->_sourceRect.h * Globals::SPRITE_SCALE);

		SDL_Rect sourceRect = this->_animations[this->_currentAnimations][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}


void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Coordinates offsets) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Coordinates>(name, offsets));
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimationOnce = once;
	if (this->_currentAnimations != animation) {
		this->_currentAnimations = animation;
		this->_frameIndex = 0;
	}

}

void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::stopAnimations() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimations);
}

void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}
