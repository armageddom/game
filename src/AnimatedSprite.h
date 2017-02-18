#ifndef ANIMATEDSTRIPE_H
#define ANIMATEDSTRIPE_H

#include "Globals.h"
#include "Sprite.h"

#include <map>
#include <string>
#include <vector>

class Graphics;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, int timeToUpdate);

	void playAnimation(std::string animation, bool once = false);
	void update(int elapsedTime);
	void draw(Graphics &graphics, float x, float y);
protected:
	int _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimations;

	virtual void addAnimation(int frames, int x, int y, std::string name, int width, int height, Coordinates offsets);
	void resetAnimations();
	void stopAnimations();
	void setVisible(bool visible);
	virtual void animationDone(std::string currentAnimation) = 0;
	virtual void setupAnimations() = 0;
private:
	std::map<std::string, std::vector<SDL_Rect>> _animations;
	std::map<std::string, Coordinates> _offsets;

	unsigned int _frameIndex;
	int _timeElapsed;
	bool _visible;
};

#endif
