#include "TileMapComponent.h"
#include "Actor.h"
#include "Math.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mTileTexture(nullptr),
	mDrawOrder(drawOrder) {

}

void TileMapComponent::Draw(class Shader* shader, class VertexArray* vertex)
{
	// プレイヤ―の位置から表示する座標を求める
	int tempTileX = (int)(mOwner->GetPosition().x) / (int)TILE_WIDTH;
	if (mScreenTileX < tempTileX)
	{
		mScreenTileX = tempTileX;
	}

	float verX = static_cast<float>(TILE_WIDTH / mTexWidth) / 2;
	float verY = static_cast<float>((float)TILE_HEIGHT / (float)mTexHeight) / 2.0f;


	// タイルマップからマップを描画
	for (size_t j = mMapDatas.size() - 1; j >= 0; j--)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (size_t x = 0; x < mMapDatas[j][y].size() + mScreenTileX; x++)
			{
				if (mMapDatas[j][y][x].verBeforeX == 0 && mMapDatas[j][y][x].verAfterX == 0)
				{
					continue;
				}

				// 描画するタイルを抜き取る
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

				Matrix4 tileTranslation = Matrix4::CreateTranslation(Vector3(mOwner->GetPosition().x + tilePositionX - 512.0f, 384.0f - (mOwner->GetPosition().y + tilePositionY), 0.0f));

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

		//ファイルを開く
		std::fstream filestream(filename);

		// ファイルが開かなかった場合は終了する
		if (!filestream.is_open())
		{
			return false;
		}

		//ファイルを読み込む
		while (!filestream.eof())
		{
			// 1行読み込む
			std::string buffer;
			filestream >> buffer;

			// ファイルから読み込んだ1行の文字列を区切り文字で分けてリストに追加する
			std::istringstream streambuffer(buffer);
			std::string token;
			int token_num;

			mTileSets.back().emplace_back();

			while (std::getline(streambuffer, token, delimiter))
			{
				token_num = atoi(token.c_str());
				// 1セルの文字列をリストに追加する
				mTileSets.back().back().emplace_back(token_num);
			}
		}
	}

	return true;
}

void TileMapComponent::SetTileMap(class Texture* tile_texture)
{
	mTileTexture = tile_texture;
	// テクスチャの幅と高さを求める
	mTexWidth = mTileTexture->GetWidth();
	mTexHeight = mTileTexture->GetHeight();

	mTexWidth = 192;
	mTexHeight = 256;

	bool status = false;
	std::vector<std::string> filenames = {
		"../Assets/MapLayer.csv"
	};

	// CSVファイルの内容を取得する
	status = GetMapLayer(filenames, ',');


	tileMapVertex temp;

	// タイルセットからタイルマップを生成
	for (auto TileSet : mTileSets)
	{
		mMapDatas.emplace_back();
		for (int i = 0; i < MAP_HEIGHT; i++)
		{

			mMapDatas.back().emplace_back();

			for (int j = 0; j < TileSet[i].size(); j++)
			{
				if (TileSet[i][j] == -1 || TileSet[i][j] == 0)
				{
					temp.verBeforeX = 0.0f;
					temp.verAfterX = 0.0f;
					temp.verBeforeY = 0.0f;
					temp.verAfterY = 0.0f;
				}
				else
				{
					temp.verBeforeX = static_cast<float>((((TileSet[i][j] - 1) % (int)(mTexWidth / TILE_WIDTH)) * (TILE_WIDTH)) / mTexWidth);
					temp.verAfterX = static_cast<float>(((((TileSet[i][j] - 1) % (int)(mTexWidth / TILE_WIDTH)) + 1) * TILE_WIDTH) / mTexWidth);
					temp.verBeforeY = static_cast<float>(((int)((TileSet[i][j] - 1) / (mTexWidth / TILE_WIDTH)) * TILE_HEIGHT) / mTexHeight);
					temp.verAfterY = static_cast<float>(((int)((TileSet[i][j] - 1) / (mTexWidth / TILE_WIDTH) + 1) * TILE_HEIGHT) / mTexHeight);
				}

				mMapDatas.back().back().emplace_back(temp);
			}
		}
	}
}