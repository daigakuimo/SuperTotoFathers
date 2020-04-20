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

	void SetChasePlayer(class Actor* player) { mChasePlayer = player; }

	class Actor* GetChasePlayer() { return mChasePlayer; }

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

	// タイルマップからBoxCompoentを作成する用
	struct rectangle
	{
		int minX;
		int maxX;
		int minY;
		int maxY;
	};

	bool mIsCreatingRect;

	std::vector<std::vector<rectangle>> mTempRects;
	std::vector<rectangle> mRects;
	std::vector<class BoxComponent*> mBoxCompList;

	class Texture* mTileTexture;
	std::vector<std::vector<std::vector<int>>> mTileSets;

	int mDrawOrder;

	// All tile map
	std::vector<std::vector<std::vector<tileMapVertex>>> mMapDatas;

	// width/height of tile set 
	int mTexWidth;
	int mTexHeight;

	// width/height of a tile
	const float TILE_HEIGHT = 64;
	const float TILE_WIDTH = 64;

	// width/height of map
	const float MAP_HEIGHT = 12;
	const float MAP_WIDTH = 16;

	// １フレーム前の描画するタイルの一番左のX座標
	int mBeforeTileX;

	class Actor* mChasePlayer;

	int checkBox(const rectangle checkRect, int checkHeight);

};
