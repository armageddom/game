#ifndef SPRITE_H
#define SPRITE_H

#include "Graphics.h"
#include "Rectangle.h"
#include "Globals.h"

#include <SDL.h>
#include <string>

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, float x, float y, bool ignoreOffset = false);

	const inline float getX() const { return this->_x; };
	const inline float getY() const { return this->_y; };

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;
	Rectangle _boundingBox;

	float _x, _y;
private:

};

#endif
