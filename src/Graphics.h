#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <map>
#include <string>

#include "Globals.h"

struct Coordinates;

class Graphics {
public:
	Graphics();
	~Graphics();

	SDL_Surface* loadImage(const std::string &filePath);
	SDL_Renderer* getRenderer() const;

	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRect, SDL_Rect* destRect, bool ignoreOffset = false);
	void flip();
	void clear();

	void setViewOffset(Coordinates center);
	Coordinates getViewOffset();

	std::map<std::string, SDL_Surface*> spriteSheet;
private:
	Coordinates _viewOffset;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};

#endif