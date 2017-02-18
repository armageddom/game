#ifndef DOOR_H
#define DOOR_H
#pragma once

#include "Rectangle.h"
#include "Globals.h"

#include <string>

class Door : public Rectangle {
public:
	Door() {}
	Door(Rectangle r, std::string destination) :
		Rectangle(
			(int)(r.getLeft() * Globals::SPRITE_SCALE), 
			(int)(r.getTop() * Globals::SPRITE_SCALE),
			(int)(r.getWidth() * Globals::SPRITE_SCALE), 
			(int)(r.getHeight() * Globals::SPRITE_SCALE)
			),
		_destination(destination)
	{}
	const inline Rectangle getRectangle() const { return this->getRect(); }
	const inline std::string getDestination() const { return this->_destination; }
private:
	std::string _destination;
};


#endif
