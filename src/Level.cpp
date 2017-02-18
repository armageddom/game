#include "Level.h"
#include "Graphics.h"
#include "Exception.h"
#include "Enemy.h"
#include "Zombie.h"

#include <SDL.h>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <math.h>
#include <tinyxml2.h>

namespace {
	const std::string MAP_FILE_EXTENSION = ".tmx";
	namespace Layer {
		const std::string BACKGROUND = "Background";
		const std::string FOREGROUND = "Foreground";
	}
	namespace ObjectGroup {
		const std::string ENEMIES = "Enemies";
		const std::string COLLISIONS = "Collisions";
		const std::string SPAWN_POINTS = "SpawnPoints";
		const std::string DOORS = "Doors";
	}
	namespace SpawnPoints {
		const std::string PLAYER = "Player";
	}
	namespace Properties {
		const std::string DESTINATION = "Destination";
	}
	namespace EnemyType {
		const std::string ZOMBIE = "Zombie";
	}
}

Level::Level() {
}

Level::Level(std::string mapName, Graphics &graphics) :
	_mapName(mapName),
	_size(Coordinates(0,0)) 
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {
}

void Level::update(int elapsedTime, Player &player) {
	for (unsigned int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
}

void Level::drawBackground(Graphics &graphics) {
	for (unsigned int i = 0; i < this->_backgroundTileList.size(); i++) {
		this->_backgroundTileList.at(i).draw(graphics);
	}
}
void Level::drawForeground(Graphics &graphics) {
	for (unsigned int i = 0; i < this->_foregroundTileList.size(); i++) {
		this->_foregroundTileList.at(i).draw(graphics);
	}
}
void Level::drawEnvironment(Graphics &graphics) {
	for (unsigned int i = 0; i < this->_environmentTileList.size(); i++) {
		this->_environmentTileList.at(i).draw(graphics);
	}
}
void Level::drawObjects(Graphics &graphics) {
	for (unsigned int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	tinyxml2::XMLDocument doc;
	std::stringstream ss;
	ss << Paths::MAPS << mapName << MAP_FILE_EXTENSION;
	if (doc.LoadFile(ss.str().c_str())) {
		throw Exception::FileNotFound(mapName);
	}

	tinyxml2::XMLElement* mapNode = doc.FirstChildElement("map");
	int mapWidth, mapHeight;
	mapNode->QueryIntAttribute("width", &mapWidth);
	mapNode->QueryIntAttribute("height", &mapHeight);
	this->_size = Coordinates(mapWidth, mapHeight);

	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Coordinates(tileWidth, tileHeight);

	// Tilesets
	tinyxml2::XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			this->parseTileset(pTileset, graphics);
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}
	// Layers
	tinyxml2::XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			this->parseTileLayer(pLayer);
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	// Parse ObjectGroups
	tinyxml2::XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			ss.str(std::string());
			ss << name;
			if (ss.str() == ObjectGroup::COLLISIONS) {
				this->parseCollisions(pObjectGroup);
			} else if (ss.str() == ObjectGroup::ENEMIES) {
				this->parseEnemies(pObjectGroup, graphics);
			} else if (ss.str() == ObjectGroup::SPAWN_POINTS) {
				this->parseSpawnPoints(pObjectGroup);
			} else if (ss.str() == ObjectGroup::DOORS) {
				this->parseDoors(pObjectGroup);
			} else {
				throw std::invalid_argument("invalid object group");
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::parseTileset(tinyxml2::XMLElement* pTileset, Graphics &graphics) {
	int firstGid;
	const char* source = pTileset->FirstChildElement("image")->Attribute("source");
	std::stringstream ss;
	ss << source;
	std::string path = ss.str();
	path.replace(0, 3, Paths::CONTENT);

	pTileset->QueryIntAttribute("firstgid", &firstGid);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(path));
	if (texture == NULL) {
		throw Exception::FileNotFound(path);
	}
	this->_tilesets.push_back(Tileset(texture, firstGid));
}

void Level::parseTileLayer(tinyxml2::XMLElement* pLayer) {
	std::stringstream ss;
	const char* name = pLayer->Attribute("name");
	ss << name;

	std::vector<Tile>* tileList;
	if (ss.str() == Layer::BACKGROUND) {
		tileList = &this->_backgroundTileList;
	} else if (ss.str() == Layer::FOREGROUND) {
		tileList = &this->_foregroundTileList;
	} else {
		tileList = &this->_environmentTileList;
	}

	tinyxml2::XMLElement* pData = pLayer->FirstChildElement("data");
	if (pData != NULL) {
		while (pData) {
			tinyxml2::XMLElement* pTile = pData->FirstChildElement("tile");
			if (pTile != NULL) {
				int tileCounter = -1;
				while (pTile) {
					tileCounter++;
					int gid = pTile->IntAttribute("gid");
					if (gid == 0) {
						if (pTile->NextSiblingElement("tile")) {
							pTile = pTile->NextSiblingElement("tile");
							continue;
						}
						break;
					}
					Tileset tileset;
					int closest = 0;
					int indexOfTileset = -1;
					for (unsigned int i = 0; i < this->_tilesets.size(); i++) {
						if (this->_tilesets[i].firstGid <= gid) {
							if (this->_tilesets[i].firstGid > closest) {
								closest = this->_tilesets[i].firstGid;
								indexOfTileset = i;
							}
						}
					}
					if (indexOfTileset >= 0) {
						tileset = this->_tilesets.at(indexOfTileset);
					}
					
					if (tileset.firstGid == -1) {
						if (pTile->NextSiblingElement("tile")) {
							pTile = pTile->NextSiblingElement("tile");
							continue;
						}
						break;
					}

					unsigned int tileX = (tileCounter % this->_size.x) * this->_tileSize.x;
					unsigned int tileY = this->_tileSize.y * (int)ceil((tileCounter / this->_size.x));
					Coordinates tilePosition(tileX, tileY);

					int tilesetWidth, tilesetHeight;
					SDL_QueryTexture(tileset.texture, NULL, NULL, &tilesetWidth, &tilesetHeight);

					unsigned int tilesetX = ((gid - tileset.firstGid) % (tilesetWidth / this->_tileSize.x)) * this->_tileSize.x;
					unsigned int tilesetY = (int)floor((gid - tileset.firstGid) / (tilesetWidth / this->_tileSize.x)) * this->_tileSize.y;
					Coordinates tilesetPosition(tilesetX, tilesetY);

					Tile tile(tileset.texture, this->_tileSize, tilesetPosition, tilePosition);
					tileList->push_back(tile);

					pTile = pTile->NextSiblingElement("tile");
				}
			}
			pData = pData->NextSiblingElement("data");
		}
	}
}

void Level::parseCollisions(tinyxml2::XMLElement* pObjectGroup) {
	tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
	if (pObject != NULL) {
		while (pObject) {
			float x, y, width, height;
			x = pObject->FloatAttribute("x");
			y = pObject->FloatAttribute("y");
			width = pObject->FloatAttribute("width");
			height = pObject->FloatAttribute("height");
			this->_collisionRects.push_back(Rectangle(
				(int)(std::ceil(x) * Globals::SPRITE_SCALE),
				(int)(std::ceil(y) * Globals::SPRITE_SCALE),
				(int)(std::ceil(width) * Globals::SPRITE_SCALE),
				(int)(std::ceil(height) * Globals::SPRITE_SCALE)
			));
			pObject = pObject->NextSiblingElement("object");
		}
	}
}

void Level::parseEnemies(tinyxml2::XMLElement* pObjectGroup, Graphics &graphics) {
	float x, y;
	tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
	if (pObject != NULL) {
		while (pObject) {
			x = pObject->FloatAttribute("x");
			y = pObject->FloatAttribute("y");
			const char* eName = pObject->Attribute("name");
			std::stringstream ss;
			ss << eName;
			if (ss.str() == EnemyType::ZOMBIE) {
				this->_enemies.push_back(
					new Zombie(graphics, Coordinates((int)(std::floor(x) * Globals::SPRITE_SCALE), (int)(std::floor(y) * Globals::SPRITE_SCALE)))
				);
			}
			pObject = pObject->NextSiblingElement("object");
		}
	}
}

void Level::parseSpawnPoints(tinyxml2::XMLElement* pObjectGroup) {
	tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
	if (pObject != NULL) {
		while (pObject) {
			float x = pObject->FloatAttribute("x");
			float y = pObject->FloatAttribute("y");
			const char* sName = pObject->Attribute("name");
			std::stringstream ss;
			ss << sName;
			if (ss.str() == SpawnPoints::PLAYER) {
				this->_spawnPoint = Coordinates((int)(std::ceil(x) * Globals::SPRITE_SCALE), (int)(std::ceil(y) * Globals::SPRITE_SCALE));
			}
			pObject = pObject->NextSiblingElement("object");
		}
	}
}

void Level::parseDoors(tinyxml2::XMLElement* pObjectGroup) {
	tinyxml2::XMLElement* pObject = pObjectGroup->FirstChildElement("object");
	if (pObject != NULL) {
		while (pObject) {
			float x = pObject->FloatAttribute("x");
			float y = pObject->FloatAttribute("y");
			float w = pObject->FloatAttribute("width");
			float h = pObject->FloatAttribute("height");
			
			Rectangle rect = Rectangle((int)x, (int)y, (int)w, (int)h);

			tinyxml2::XMLElement* pProperties = pObject->FirstChildElement("properties");
			if (pProperties != NULL) {
				while (pProperties) {
					tinyxml2::XMLElement* pProperty = pProperties->FirstChildElement("property");
					if (pProperty != NULL) {
						while (pProperty) {
							const char* propName = pProperty->Attribute("name");
							std::stringstream ss;
							ss << propName;
							if (ss.str() == "Destination") {
								const char* value = pProperty->Attribute("value");
								ss.str(std::string());
								ss << value;

								Door door = Door(rect, ss.str());
								this->_doorList.push_back(door);
							}
							pProperty = pProperty->NextSiblingElement("property");
						}
					}
					pProperties = pProperties->NextSiblingElement("properties");
				}
			}
			pObject = pObject->NextSiblingElement("object");
		}
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle & other) {
	std::vector<Rectangle> others;
	for (unsigned int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollision(const Rectangle &other) {
	std::vector<Door> others;
		for (unsigned int i = 0; i < this->_doorList.size(); i++) {
			if (this->_doorList.at(i).collidesWith(other)) {
				others.push_back(this->_doorList.at(i));
			}
		}
	return others;
}

std::vector<Enemy*> Level::checkEnemyCollision(const Rectangle &other) {
	std::vector<Enemy*> others;
	for (unsigned int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}

const Coordinates Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}