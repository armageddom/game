#include <SDL_image.h>

#include "Graphics.h"
#include "Globals.h"
#include "Exception.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Peli");
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (this->spriteSheet.count(filePath) == 0) {
		this->spriteSheet[filePath] = IMG_Load(filePath.c_str());
	}
	if (this->spriteSheet[filePath] == NULL) {
		throw Exception::FileNotFound("image not found");
	}
	return this->spriteSheet[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect, bool ignoreOffset) {
	if (sourceRect->x < 0 || sourceRect->y < 0 || sourceRect->w < 0 || sourceRect->h < 0) {
		throw std::range_error("source rectangle invalid coordinates");
	}
	if (! ignoreOffset) {
		destRect->x += this->_viewOffset.x;
		destRect->y += this->_viewOffset.y;
	}
	SDL_RenderCopy(this->_renderer, texture, sourceRect, destRect);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->_renderer;
}

void Graphics::setViewOffset(Coordinates offset) {
	this->_viewOffset = offset;
}

Coordinates Graphics::getViewOffset() {
	return this->_viewOffset;
}