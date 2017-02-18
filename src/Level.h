#ifndef LEVEL_H
#define LEVEL_H

#include "Rectangle.h"
#include "Globals.h"
#include "Graphics.h"
#include "Tile.h"
#include "Door.h"
#include "Player.h"
#include "Enemy.h"

#include <string>
#include <vector>
#include <tinyxml2.h>

class Graphics;
class Enemy;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName,Graphics &graphics);
	~Level();
	void update(int elapsedTime, Player &player);
	void drawBackground(Graphics &graphics);
	void drawForeground(Graphics &graphics);
	void drawEnvironment(Graphics &graphics);
	void drawObjects(Graphics &graphics);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	std::vector<Door> checkDoorCollision(const Rectangle &other);
	std::vector<Enemy*> checkEnemyCollision(const Rectangle &other);

	const Coordinates getPlayerSpawnPoint() const;

private:
	void loadMap(std::string mapName, Graphics &graphics);
	void parseTileset(tinyxml2::XMLElement* pTileset, Graphics &graphics);
	void parseTileLayer(tinyxml2::XMLElement* pObjectGroup);
	void parseCollisions(tinyxml2::XMLElement* pObjectGroup);
	void parseEnemies(tinyxml2::XMLElement* pObjectGroup, Graphics &graphics);
	void parseSpawnPoints(tinyxml2::XMLElement* pObjectGroup);
	void parseDoors(tinyxml2::XMLElement* pObjectGroup);

	std::string _mapName;
	Coordinates _spawnPoint;
	Coordinates _size;
	Coordinates _tileSize;
	SDL_Texture* _backgroundTexture;
	std::vector<Tile> _backgroundTileList;
	std::vector<Tile> _foregroundTileList;
	std::vector<Tile> _environmentTileList;
	std::vector<Tileset> _tilesets;	
	std::vector<Rectangle>_collisionRects;
	std::vector<Door> _doorList;
	std::vector<Enemy*> _enemies;
};

struct Tileset {
	SDL_Texture* texture;
	int firstGid;
	Tileset() {
		this->firstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->texture = texture;
		this->firstGid = firstGid;
	}
};
#endif