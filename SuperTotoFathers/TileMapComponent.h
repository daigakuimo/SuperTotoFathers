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
	void Draw(class Shader* shader, class VertexArray* vertex) override;

	// Create tile map from tile set
	void SetTileMap(class Texture* tile_texture);

	// Get tile set from csv file
	bool GetMapLayer(const std::vector<std::string>& filenames, const char delimiter = ',');

private:
	struct tileMapVertex
	{
		float verBeforeX;
		float verBeforeY;
		float verAfterX;
		float verAfterY;
	};


	class Texture* mTileTexture;
	std::vector<std::vector<int>> mTileSets;

	int mDrawOrder;

	// All tile map
	std::vector<std::vector<tileMapVertex>> mMapDatas;

	// width/height of tile set 
	int mTexWidth;
	int mTexHeight;

	// width/height of a tile
	const float TILE_HEIGHT = 64;
	const float TILE_WIDTH = 64;

	// width/height of map
	const float MAP_HEIGHT = 12;
	const float MAP_WIDTH = 16;
};
