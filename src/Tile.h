#pragma once

#include "Globals.h"
#include "Graphics.h"

class Graphics;
struct SDL_Texture;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Coordinates size, Coordinates tilesetPosition, Coordinates position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
private:
	SDL_Texture* _tileset;
	Coordinates _tilesetPosition;
	Coordinates _position;
	Coordinates _size;
};

