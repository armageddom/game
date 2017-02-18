#pragma once

#include "Sprite.h"
#include "Graphics.h"

class Graphics;

class Bar : public Sprite {
public:
	Bar();
	Bar(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, float posX, float posY);
	void setWidth(int current, int max);
	void draw(Graphics &graphics, float x, float y);
protected:
	Sprite _background;
	int _defaultWidth;
};