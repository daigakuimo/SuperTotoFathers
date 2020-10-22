#include "TileMapComponent.h"
#include "Actor.h"
#include "Math.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "BoxComponent.h"
#include "Game.h"
#include "Goal.h"

TileMapComponent::TileMapComponent(class Actor* owner, std::vector<std::string> filenames, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mTileTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexHeight(0)
	, mTexWidth(0)
	, mBeforeTileX(0)
	, mIsCreatingRect(false)
	, mFilenames(filenames)
{

}

void TileMapComponent::Draw(class Shader* shader, class VertexArray* vertex)
{
	// �v���C���\�̈ʒu����\��������W�����߂�
	int tileX = ((int)(GetChasePlayer()->GetPosition().x) / (int)TILE_WIDTH) + 1;

	if (mBeforeTileX > tileX)
	{
		tileX = mBeforeTileX;
	}

	mBeforeTileX = tileX;

	float verX = static_cast<float>(TILE_WIDTH / mTexWidth) / 2;
	float verY = static_cast<float>((float)TILE_HEIGHT / (float)mTexHeight) / 2.0f;


	// �^�C���}�b�v����}�b�v��`��
	for (int j = mMapDatas.size() - 1; j >= 0; j--)
	{
		for (int y = 0; y < MAP_HEIGHT + 1; y++)
		{
			for (size_t x = tileX; x < tileX + MAP_WIDTH + 4; x++)
			{
				if (mMapDatas[j][y][x].verBeforeX == 0 && mMapDatas[j][y][x].verAfterX == 0)
				{
					continue;
				}

				// �`�悷��^�C���𔲂����
				float vertices[] = {
					-verX,  verY, 0.f, mMapDatas[j][y][x].verBeforeX, mMapDatas[j][y][x].verBeforeY, // top left
					 verX,  verY, 0.f, mMapDatas[j][y][x].verAfterX , mMapDatas[j][y][x].verBeforeY, // top right
					 verX, -verY, 0.f, mMapDatas[j][y][x].verAfterX , mMapDatas[j][y][x].verAfterY, // bottom right
					-verX, -verY, 0.f, mMapDatas[j][y][x].verBeforeX, mMapDatas[j][y][x].verAfterY  // bottom left
				};


				vertex->ChangeVBO(vertices);
				vertex->SetActive();

				float tilePositionX = static_cast<float>(x * TILE_WIDTH);
				float tilePositionY = static_cast<float>(y * TILE_HEIGHT);


				// Scale the quad by the width/height of texture

				Matrix4 scaleMat = Matrix4::CreateScale(
					static_cast<float>(mTexWidth),
					static_cast<float>(mTexHeight),
					1.0f);

				Matrix4 tileTranslation = Matrix4::CreateTranslation(Vector3(tilePositionX - 512.0f, 384.0f - tilePositionY, 0.0f));

				Matrix4 world = scaleMat * tileTranslation;


				// Since all sprites use the same shader/vertices,
				// the game first sets them active before any sprite draws

				// Set world transform
				shader->SetMatrixUniform("uWorldTransform", world);

				mTileTexture->SetActive();

				// Draw quad
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			}

		}
	}
}

bool TileMapComponent::GetMapLayer(const std::vector<std::string> & filenames, const char delimiter)
{
	for (std::string filename : filenames)
	{

		mTileSets.emplace_back();

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

			mTileSets.back().emplace_back();

			while (std::getline(streambuffer, token, delimiter))
			{
				token_num = atoi(token.c_str());
				// 1�Z���̕���������X�g�ɒǉ�����
				mTileSets.back().back().emplace_back(token_num);
			}
		}
	}

	return true;
}


void TileMapComponent::SetTileMap(class Texture* tile_texture)
{
	mTileTexture = tile_texture;
	// �e�N�X�`���̕��ƍ��������߂�
	mTexWidth = mTileTexture->GetWidth();
	mTexHeight = mTileTexture->GetHeight();

	mTexWidth = 192;
	mTexHeight = 256;

	bool status = false;


	// CSV�t�@�C���̓��e���擾����
	status = GetMapLayer(mFilenames, ',');

	rectangle rect;

	tileMapVertex temp;

	// �^�C���Z�b�g����^�C���}�b�v�𐶐�
	for (auto TileSet : mTileSets)
	{
		mMapDatas.emplace_back();
		for (int i = 0; i < MAP_HEIGHT + 1; i++)
		{

			mMapDatas.back().emplace_back();

			mTempRects.emplace_back();

			mIsCreatingRect = false;

			for (int j = 0; j < TileSet[i].size(); j++)
			{
				if (TileSet[i][j] == -1 || TileSet[i][j] == 0)
				{
					temp.verBeforeX = 0.0f;
					temp.verAfterX = 0.0f;
					temp.verBeforeY = 0.0f;
					temp.verAfterY = 0.0f;

					mIsCreatingRect = false;
				}
				else
				{
					temp.verBeforeX = static_cast<float>((((TileSet[i][j] - 1) % (int)(mTexWidth / TILE_WIDTH)) * (TILE_WIDTH)) / mTexWidth);
					temp.verAfterX = static_cast<float>(((((TileSet[i][j] - 1) % (int)(mTexWidth / TILE_WIDTH)) + 1) * TILE_WIDTH) / mTexWidth);
					temp.verBeforeY = static_cast<float>(((int)((TileSet[i][j] - 1) / (mTexWidth / TILE_WIDTH)) * TILE_HEIGHT) / mTexHeight);
					temp.verAfterY = static_cast<float>(((int)((TileSet[i][j] - 1) / (mTexWidth / TILE_WIDTH) + 1) * TILE_HEIGHT) / mTexHeight);

					// BoxComponent�쐬�̂��߁A��`���쐬
					if (!(TileSet[i][j] == 3 || TileSet[i][j] == 4 || TileSet[i][j] == 5 || TileSet[i][j] == 6))
					{
						if (mIsCreatingRect)
						{
							mTempRects.back().back().maxX = j;
						}
						else
						{
							mIsCreatingRect = true;
							rect.minX = j;
							rect.maxX = j;
							rect.minY = i;
							rect.maxY = i;
							mTempRects.back().emplace_back(rect);
						}
					}
				}

				mMapDatas.back().back().emplace_back(temp);
			}
		}
	}

	// ��s���Ƃɂł�����`���������Ă��ꂼ��1�̋�`�ɂ���
	for (size_t h = 0; h < mTempRects.size(); h++)
	{
		for (size_t w = 0; w < mTempRects[h].size(); w++)
		{
			mRects.emplace_back(mTempRects[h][w]);
			checkBox(mTempRects[h][w], h + 1);
		}
	}

	for (size_t i = 0; i < mRects.size(); i++)
	{
		class BoxComponent* boxComp = new BoxComponent(mOwner);
		AABB tileBox(Vector3(mRects[i].minX * 64.0f - 512.0f - 32.0f , 384.0f - (mRects[i].maxY + 1) * 64.0f + 32.0f,0),
			Vector3((mRects[i].maxX + 1) * 64.0f - 512.0f - 32.0f, 384.0f - (mRects[i].minY) * 64.0f + 32.0f,0));

		boxComp->SetObjectBox(tileBox);
		boxComp->SetShouldRotate(false);

		mBoxCompList.emplace_back(boxComp);

		mOwner->GetGame()->SetStageBoxes(boxComp);
	}
}

int TileMapComponent::checkBox(const rectangle checkRect, int checkHeight)
{
	bool isMatch = false;
	std::vector<int> deleteNumber;
	for (size_t w = 0; w < mTempRects[checkHeight].size(); w++)
	{
		if (mTempRects[checkHeight][w].minX == checkRect.minX && mTempRects[checkHeight][w].maxX == checkRect.maxX)
		{
			mRects.back().maxY = mTempRects[checkHeight][w].maxY;
			isMatch = true;

			// �������ꂽ��`�͏���
			deleteNumber.emplace_back(w);
			
			break;
		}
	}


	if (checkHeight + 1 < mTempRects.size())
	{
		if (isMatch)
		{
			checkBox(checkRect, checkHeight + 1);
		}
	}

	for (auto iter = deleteNumber.begin(); iter != deleteNumber.end(); ++iter)
	{
		mTempRects[checkHeight].erase(mTempRects[checkHeight].begin() + *iter);
	}

	return 1;
}

