#ifndef HUD_H
#define HUD_H

#include "Graphics.h"
#include "Sprite.h"
#include "Player.h"
#include "Bar.h"

class HUD {
public:
	HUD();
	HUD(Graphics &graphics);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
private:
	Player _player;

	int _playerMaxHealth;
	int _playerMaxSanity;
	int _playerCurrentHealth;
	int _playerCurrentSanity;

	Bar _healthBar;
	Bar _sanityBar;
};

#endif