#pragma once
#include "Actor.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class StageManager : public Actor
{
public:
	StageManager(class Game* game);

	void UpdateActor(float deltaTime) override;

	bool GetStageLayer(const std::vector<std::string>& filenames, const char delimiter = ',');

	class Actor* GetPlayer() { return mPlayer; }
	void SetPlayer(Actor* player) { mPlayer = player; }

private:
	struct ObjectPos
	{
		float x;
		float y;
		int   ObjectType;
	};

	// ブロックのタイルマップ用の二次元配列
	std::vector<std::vector<int>> mTileBrockSets;

	// 敵の位置用の二次元配列
	std::vector<std::vector<int>> mEnemysSets;

	// 生成するオブジェクトの位置と種類を格納する
	std::vector < std::vector<ObjectPos>> mBrockList;
	std::vector < std::vector<ObjectPos>> mEnemysList;


	// 操作しているプレイヤー
	class Actor* mPlayer;

	// 操作しているプレイヤーの前回いたタイルの位置(一番左がx=0, 一番上がy=0)
	int mBeforeTileX;

	const int TILE_WIDTH = 64;
};