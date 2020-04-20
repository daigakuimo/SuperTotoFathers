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

	// �u���b�N�̃^�C���}�b�v�p�̓񎟌��z��
	std::vector<std::vector<int>> mTileBrockSets;

	// �G�̈ʒu�p�̓񎟌��z��
	std::vector<std::vector<int>> mEnemysSets;

	// ��������I�u�W�F�N�g�̈ʒu�Ǝ�ނ��i�[����
	std::vector < std::vector<ObjectPos>> mBrockList;
	std::vector < std::vector<ObjectPos>> mEnemysList;


	// ���삵�Ă���v���C���[
	class Actor* mPlayer;

	// ���삵�Ă���v���C���[�̑O�񂢂��^�C���̈ʒu(��ԍ���x=0, ��ԏオy=0)
	int mBeforeTileX;

	const int TILE_WIDTH = 64;
};