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

void TileMapComponent::Draw(class Shader* shader, class VertexArray* vertex){

	float x = static_cast<float>(TILE_WIDTH / mTexWidth) / 2;
	float y = static_cast<float>((float)TILE_HEIGHT / (float)mTexHeight) / 2.0f;

	SDL_Log("x : %4.3f", x);
	SDL_Log("y : %4.4f", y);

	// �^�C���}�b�v����}�b�v��`��
	for (int j = mMapDatas.size() - 1; j >= 0; j--) {
		for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {

			// �`�悷��^�C���𔲂����
			float vertices[] = {
				-x,  y, 0.f, mMapDatas[j][i].verBeforeX, mMapDatas[j][i].verBeforeY, // top left
				 x,  y, 0.f, mMapDatas[j][i].verAfterX , mMapDatas[j][i].verBeforeY, // top right
				 x, -y, 0.f, mMapDatas[j][i].verAfterX , mMapDatas[j][i].verAfterY, // bottom right
				-x, -y, 0.f, mMapDatas[j][i].verBeforeX, mMapDatas[j][i].verAfterY  // bottom left
			};

			vertex->ChangeVBO(vertices);
			vertex->SetActive();

			float tilePositionX = static_cast<float>((i % MAP_WIDTH) * TILE_WIDTH);
			float tilePositionY = static_cast<float>((i / MAP_WIDTH) * TILE_HEIGHT);

			// Scale the quad by the width/height of texture

			Matrix4 tileTranslation = Matrix4::CreateTranslation(Vector3(mOwner->GetPosition().x + tilePositionX - 512.0f, tilePositionY - 384.0f, 0.0f));

			Matrix4 world = tileTranslation;


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

bool TileMapComponent::GetMapLayer(const std::vector<std::string> & filenames, const char delimiter) {
	for (std::string filename : filenames) {

		mTileSets.emplace_back();

		//�t�@�C�����J��
		std::fstream filestream(filename);

		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		if (!filestream.is_open()) {
			return false;
		}

		//�t�@�C����ǂݍ���
		while (!filestream.eof()) {
			// 1�s�ǂݍ���
			std::string buffer;
			filestream >> buffer;

			// �t�@�C������ǂݍ���1�s�̕��������؂蕶���ŕ����ă��X�g�ɒǉ�����
			std::istringstream streambuffer(buffer);
			std::string token;
			int token_num;

			while (std::getline(streambuffer, token, delimiter)) {
				token_num = atoi(token.c_str());
				// 1�Z���̕���������X�g�ɒǉ�����
				mTileSets.back().emplace_back(token_num);
			}
		}
	}

	return true;
}

void TileMapComponent::SetTileMap(class Texture* tile_texture) {
	mTileTexture = tile_texture;
	// �e�N�X�`���̕��ƍ��������߂�
	mTexWidth = mTileTexture->GetWidth();
	mTexHeight = mTileTexture->GetHeight();

	mTexWidth = 64;
	mTexHeight = 128;

	bool status = false;
	std::vector<std::string> filenames = {
		"../Assets/MapLayer.csv"
	};

	// CSV�t�@�C���̓��e���擾����
	status = GetMapLayer(filenames, ',');


	tileMapVertex temp;

	// �^�C���Z�b�g����^�C���}�b�v�𐶐�
	for (auto TileSet : mTileSets) {
		mMapDatas.emplace_back();
		for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {

			temp.verBeforeX = static_cast<float>(((TileSet[i] % (mTexWidth / TILE_WIDTH)) * TILE_WIDTH) / mTexWidth);
			temp.verAfterX = static_cast<float>((((TileSet[i] % (mTexWidth / TILE_WIDTH)) + 1) * TILE_WIDTH) / mTexWidth);
			temp.verBeforeY = static_cast<float>((((TileSet[i] / (mTexWidth / TILE_WIDTH)) - 1) * TILE_HEIGHT) / mTexHeight);
			temp.verAfterY = static_cast<float>(((TileSet[i] / (mTexWidth / TILE_WIDTH)) * TILE_HEIGHT) / mTexHeight);

			mMapDatas.back().emplace_back(temp);
		}
	}
}