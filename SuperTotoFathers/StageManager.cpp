#include "StageManager.h"
#include "Brock.h"
#include "SDL_log.h"
#include "Goomba.h"
#include "Goal.h"

StageManager::StageManager(class Game* game)
	:Actor(game)
	, mPlayer(nullptr)
	, mBeforeTileX(-100)
{
	std::vector<std::string> filenames = {
		"../Assets/BrockLayer.csv",
		"../Assets/EnemysLayer.csv"
	};

	GetStageLayer(filenames);

}

void StageManager::UpdateActor(float deltaTime)
{
	// �v���C���[�̈ʒu����u���b�N/�G�𐶐����邩���肷��
	// ���������I�u�W�F�N�g�͔z�񂩂�폜����
	Vector2 playerPos = Vector2(mPlayer->GetPosition().x, mPlayer->GetPosition().y);

	int tileX = ((int)playerPos.x) / (int)(TILE_WIDTH);

	if (mBeforeTileX < tileX)
	{
		std::vector<std::vector<int>> deleteBrock;
		for (size_t y = 0; y < mBrockList.size(); y++)
		{
			deleteBrock.emplace_back();
			for (size_t x = 0; x < mBrockList[y].size(); x++)
			{
				if (mBrockList[y][x].x <= playerPos.x + 700.0f)
				{
					// �u���b�N�C���X�^���X�쐬
					Brock* brock = new Brock(GetGame(), mBrockList[y][x].ObjectType);
					brock->SetPosition(Vector2(mBrockList[y][x].x, mBrockList[y][x].y));

					// mBrockList����폜����v�f������
					deleteBrock.back().emplace_back(x);
				}
				else
				{
					break;
				}
			}
		}

		// �쐬�����u���b�N�̓��X�g����폜
		for (size_t y = 0; y < deleteBrock.size(); y++)
		{
			for (size_t i = 0; i < deleteBrock[y].size(); i++)
			{
				mBrockList[y].erase(mBrockList[y].begin() + deleteBrock[y][i]);
			}
		}

		std::vector<std::vector<int>> deleteEnemy;
		for (size_t y = 0; y < mEnemysSets.size(); y++)
		{
			deleteEnemy.emplace_back();
			for (size_t x = 0; x < mEnemysList[y].size(); x++)
			{
				if (mEnemysList[y][x].x <= playerPos.x + 700.0f)
				{
					// �G�L�����̃C���X�^���X�쐬
					class Goomba* mGoomba = new Goomba(GetGame());
					mGoomba->SetPosition(Vector2(mEnemysList[y][x].x, mEnemysList[y][x].y));
					mGoomba->SetScale(1.0f);

					// mEnemysList����폜����v�f������
					deleteEnemy.back().emplace_back(x);
				}
				else
				{
					break;
				}
			}
		}

		// �쐬�����G�̓��X�g����폜
		for (size_t y = 0; y < deleteEnemy.size(); y++)
		{
			for (size_t i = 0; i < deleteEnemy[y].size(); i++)
			{
				mEnemysList[y].erase(mEnemysList[y].begin() + deleteEnemy[y][i]);
			}
		}


		mBeforeTileX = tileX;
	}

}


bool StageManager::GetStageLayer(const std::vector<std::string>& filenames, const char delimiter)
{
	int c = 0;
	for (std::string filename : filenames)
	{
		//�t�@�C�����J��
		std::fstream filestream(filename);

		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		if (!filestream.is_open())
		{
			return false;
		}

		//�t�@�C����ǂݍ���
		while (!filestream.eof())
		{
			// 1�s�ǂݍ���
			std::string buffer;
			filestream >> buffer;

			// �t�@�C������ǂݍ���1�s�̕��������؂蕶���ŕ����ă��X�g�ɒǉ�����
			std::istringstream streambuffer(buffer);
			std::string token;
			int token_num;

			if (c == 0)
			{
				mTileBrockSets.emplace_back();
			}
			else
			{
				mEnemysSets.emplace_back();
			}

			while (std::getline(streambuffer, token, delimiter))
			{
				token_num = atoi(token.c_str());
				// 1�Z���̕���������X�g�ɒǉ�����
				if (c == 0)
				{
					mTileBrockSets.back().emplace_back(token_num);
				}
				else
				{
					mEnemysSets.back().emplace_back(token_num);
				}
			}
		}

		c++;
	}

	for (size_t y = 0; y < mTileBrockSets.size() - 1; y++)
	{
		mBrockList.emplace_back();
		for (size_t x = 0; x < mTileBrockSets[y].size(); x++)
		{
			if (mTileBrockSets[y][x] != -1)
			{
				if (mTileBrockSets[y][x] == 10)
				{
					// �S�[���C���X�^���X�쐬
					class Goal* goal = new Goal(GetGame());
					goal->SetPosition(Vector2(x * TILE_WIDTH - 512.0f, 384.0f - y * TILE_WIDTH + 32.0f));
					goal->SetScale(1.0f);
					goal->CreateFlag();
					GetGame()->SetGoal(goal);

				}
				else
				{
					ObjectPos temp;
					temp.x = x * TILE_WIDTH - 512.0f;
					temp.y = 384.0f - y * TILE_WIDTH;
					temp.ObjectType = mTileBrockSets[y][x];
					mBrockList.back().emplace_back(temp);
				}
			}
		}
	}

	for (size_t y = 0; y < mEnemysSets.size(); y++)
	{
		mEnemysList.emplace_back();
		for (size_t x = 0; x < mEnemysSets[y].size(); x++)
		{
			if (mEnemysSets[y][x] != -1)
			{
				ObjectPos temp;
				temp.x = x * TILE_WIDTH - 512.0f;
				temp.y = 384.0f - y * TILE_WIDTH;
				temp.ObjectType = mTileBrockSets[y][x];
				mEnemysList.back().emplace_back(temp);
			}
		}
	}

	return true;
}