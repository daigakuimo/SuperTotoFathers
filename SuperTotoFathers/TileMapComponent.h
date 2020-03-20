#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);

	// void Update(float deltaTime) override
	void Draw(class Shader* shader) override;

	// Create tile map from tile set
	void SetTileMap(class Texture* tile_texture);

	// Get tile set from csv file
	bool GetMapLayer(const std::vector<std::string>& filenames, const char delimiter = ',');

private:
	class Texture* mTileTexture;
	std::vector<std::vector<int>> mTileSets;

	int mDrawOrder;

	// All tile map
	std::vector<std::vector<SDL_Rect>> mMapDatas;

	// width/height of tile set 
	int mTexWidth;
	int mTexHeight;

	// width/height of a tile
	const int TILE_HEIGHT = 32;
	const int TILE_WIDTH = 32;

	// width/height of map
	const int MAP_HEIGHT = 24;
	const int MAP_WIDTH = 32;
};
