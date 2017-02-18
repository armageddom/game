#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <limits.h>
#include <stdexcept>

namespace Globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 600;
	const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
	const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

	const float SPRITE_SCALE = 2.0f;
}
namespace Paths {
	const std::string CONTENT = "content/";
	const std::string TILESETS = "content/tilesets/";
	const std::string SPRITES = "content/sprites/";
	const std::string MAPS = "content/maps/";
}
namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};
	const inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT, RIGHT, UP, DOWN
};
struct Coordinates {
	int x, y;
	Coordinates() : x(0), y(0) {
	}
	Coordinates(int x, int y) : x(x), y(y) {
		int range_minimum = INT_MIN / 4;
		int range_maximum = INT_MAX / 4;
		if (x <= range_minimum || y <= range_minimum || x >= range_maximum || y >= range_maximum) {
			throw std::range_error("Invalid coordinates");
		}
	}
};

#endif
