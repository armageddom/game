#include "Tile.h"
#include "Exception.h"
#include <SDL.h>

Tile::Tile() {
}

Tile::Tile(SDL_Texture* tileset, Coordinates size, Coordinates tilesetPosition, Coordinates position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Coordinates((int)(position.x * Globals::SPRITE_SCALE), (int)(position.y * Globals::SPRITE_SCALE))) 
{
	if (this->_tileset == NULL) {
		throw Exception::FileNotFound("tileset not found");
	}
}

void Tile::update(int elapsedTime) {
	(void)elapsedTime;
}

void Tile::draw(Graphics &graphics) {
	SDL_Rect destRect = {
		this->_position.x, this->_position.y, (int)(this->_size.x * Globals::SPRITE_SCALE), (int)(this->_size.y * Globals::SPRITE_SCALE) 
	};
	SDL_Rect sourceRect = {
		this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y 
	};
	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}
