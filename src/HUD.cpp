#include "HUD.h"
#include "Globals.h"

namespace {
	const std::string HEALTH_BAR_FILE = "content/sprites/hud/health_bar.png";
	const std::string SANITY_BAR_FILE = "content/sprites/hud/sanity_bar.png";
	const int BAR_WIDTH = 192;
	const int BAR_HEIGHT = 16;
	const int BAR_POSITION_X = 0;
	const int BAR_POSITION_Y = 512;
}

HUD::HUD() {
}

HUD::HUD(Graphics &graphics) {
	this->_healthBar = Bar(graphics, HEALTH_BAR_FILE, 0, 0, BAR_WIDTH, BAR_HEIGHT, (float)BAR_POSITION_X, (float)BAR_POSITION_Y);
	this->_sanityBar = Bar(graphics, SANITY_BAR_FILE, 0, 0, BAR_WIDTH, BAR_HEIGHT, (float)BAR_POSITION_X, (float)(BAR_POSITION_Y + BAR_HEIGHT * Globals::SPRITE_SCALE));
}

void HUD::update(int elapsedTime, Player &player) {
	(void)elapsedTime;

	this->_player = player;

	this->_healthBar.setWidth(player.getCurrentHealth(), player.getMaxHealth());
	this->_sanityBar.setWidth(player.getCurrentSanity(), player.getMaxSanity());
}

void HUD::draw(Graphics &graphics) {
	this->_healthBar.draw(graphics, (float)BAR_POSITION_X, (float)BAR_POSITION_Y);
	this->_sanityBar.draw(graphics, (float)BAR_POSITION_X, (float)(BAR_POSITION_Y + BAR_HEIGHT * Globals::SPRITE_SCALE));
}