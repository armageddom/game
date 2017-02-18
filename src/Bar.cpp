#include "Bar.h"
#include <string>

namespace {
	const std::string BACKGROUND_FILE = "content/sprites/hud/bar_background.png";
}

Bar::Bar() {
}

Bar::Bar(Graphics &graphics, const std::string &filepath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	Sprite(graphics, filepath, sourceX, sourceY, width, height, posX, posY)
{
	this->_defaultWidth = width;
	this->_background = Sprite(graphics, BACKGROUND_FILE, 0, 0, width, height, posX, posY);
}

void Bar::setWidth(int current, int max) {
	int percentage = (int)((float)current / (float)max * 100);
	percentage = percentage > 100 ? 100 : percentage;
	percentage = percentage < 0 ? 0 : percentage;
	int width = (int)((float)this->_defaultWidth * ((float)percentage / 100));
	this->_sourceRect.w = width;
}

void Bar::draw(Graphics &graphics, float x, float y) {
	this->_background.draw(graphics, x, y, true);
	Sprite::draw(graphics, x, y, true);
}