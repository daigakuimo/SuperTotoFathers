#include "Player.h"
#include "Game.h"
#include "Texture.h"
#include "InputComponent.h"
#include "AnimSpriteComponent.h"
#include "CameraActor.h"
#include "BoxComponent.h"
#include "PhysWorld.h"

int c = 0;

Player::Player(Game* game)
	:Actor(game)
{
	// Create a sprite component
	//SpriteComponent* sc = new SpriteComponent(this, 150);
	//sc->SetTexture(game->GetTexture("../Assets/TOTO1-1.png"));

	// Create an animation sprite compoent
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<class Texture*> anims = {
		game->GetTexture("../Assets/Toto1-1.png"),
		game->GetTexture("../Assets/Toto1-2.png"),
		game->GetTexture("../Assets/Toto1-3.png"),
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimRange(ActionState::EStop, 0, 0, true);
	asc->SetAnimRange(ActionState::EWalk, 1, 2, true);
	asc->SetAnimRange(ActionState::EJump, 1, 2, true);
	asc->SetAnimRange(ActionState::EFall, 0, 0, true);

	// Create an input component and set keys/speed
	ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_D);
	ic->SetBackKey(SDL_SCANCODE_A);
	ic->SetJumpKey(SDL_SCANCODE_SPACE);
	ic->SetDashKey(SDL_SCANCODE_L);
	ic->SetMaxJumpHeight(192.0f);
	ic->SetMass(1.0f);
	ic->SetVelocityLimit(Vector2(400.0f, 400.0f));
	ic->SetIsGravity(true);

	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-15.0f, -32.0f, 0.f),
		Vector3(15.0f, 32.0f, 0.f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

}

void Player::UpdateActor(float deltaTime)
{
	mBoxComp->OnUpdateWorldTransform();
	// 地面にいるかどうか判定
	Vector3 start = Vector3(GetPosition().x, GetPosition().y, 0);
	Vector3 dir = Vector3(0, -1, 0);
	Vector3 end = start + dir * mDOWN_SEGMENT_LENGTH;
	LineSegment downLs(start, end);

	PhysWorld* phys = GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	if (phys->AvoidActorSegmentCast(downLs, info, this))
	{
		if (Math::NearZero(ic->GetForwardSpeed().x))
		{
			SetActionState(ActionState::EStop);
		}
		else
		{
			SetActionState(ActionState::EWalk);
		}
	}
	else
	{
		SetActionState(ActionState::EJump);
	}


	std::vector<class BoxComponent*> boxList = GetGame()->GetStageBoxes();
	
	for (auto box : boxList)
	{
		// 当たり判定
		Vector2 pos = GetPosition();

		const AABB& stageBox = box->GetWorldBox();
		const AABB& playerBox = mBoxComp->GetWorldBox();

		if (Intersect(playerBox, stageBox))
		{
			// どこに衝突したか判定
			Vector3 lineStart = Vector3(mPrevPos.x, mPrevPos.y, 0);
			Vector3 lineEnd   = Vector3(GetPosition().x, GetPosition().y, 0);
			LineSegment line(start, end);

			float t;

			Vector3 norm;

			float dx = 0.0f;
			float dy = 0.0f;

			if (Intersect(line, stageBox, t, norm))
			{

				if (norm ==  Vector3::NegUnitX)
				{
					// オブジェクトの左側に当たった
					float dy1 = stageBox.mMin.y - playerBox.mMax.y;
					float dy2 = stageBox.mMax.y - playerBox.mMin.y;

					dx = stageBox.mMin.x - playerBox.mMax.x;
					dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
				}
				else if (norm == Vector3::UnitX)
				{
					// オブジェクトの右側に当たった
					float dy1 = stageBox.mMin.y - playerBox.mMax.y;
					float dy2 = stageBox.mMax.y - playerBox.mMin.y;

					dx = stageBox.mMax.x - playerBox.mMin.x;
					dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
				}
				else if (norm == Vector3::NegUnitY)
				{
					// オブジェクトの下側に当たった
					float dx1 = stageBox.mMin.x - playerBox.mMax.x;
					float dx2 = stageBox.mMax.x - playerBox.mMin.x;

					// 絶対値の小さい方をセットする
					dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
					dy = stageBox.mMin.y - playerBox.mMax.y;
				}
				else if (norm == Vector3::UnitY)
				{
					// オブジェクトの上側に当たった
					float dx1 = stageBox.mMin.x - playerBox.mMax.x;
					float dx2 = stageBox.mMax.x - playerBox.mMin.x;
					
					// 絶対値の小さい方をセットする
					dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
					dy = stageBox.mMax.y - playerBox.mMin.y;
				}
				else
				{
					// 全ての差を計算する
					float dx1 = stageBox.mMin.x - playerBox.mMax.x;
					float dx2 = stageBox.mMax.x - playerBox.mMin.x;
					float dy1 = stageBox.mMin.y - playerBox.mMax.y;
					float dy2 = stageBox.mMax.y - playerBox.mMin.y;


					// 絶対値の小さい方をセットする
					dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
					dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
				}

			}
			else
			{
				// 全ての差を計算する
				float dx1 = stageBox.mMin.x - playerBox.mMax.x;
				float dx2 = stageBox.mMax.x - playerBox.mMin.x;
				float dy1 = stageBox.mMin.y - playerBox.mMax.y;
				float dy2 = stageBox.mMax.y - playerBox.mMin.y;


				// 絶対値の小さい方をセットする
				dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
				dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
			}

			// x/yのうち一番小さい軸で位置を調整
			if (Math::Abs(dx) < Math::Abs(dy))
			{
				pos.x += dx;
			}
			else
			{
				pos.y += dy;
			}

			// 位置を調整
			SetPosition(pos);
			mBoxComp->OnUpdateWorldTransform();

		}
	}

	// 画面の左端では止まる
	float cameraPos = GetGame()->GetCameraActor()->GetBeforeX();
	Vector2 currentPos = GetPosition();
	if (currentPos.x < -cameraPos - 445.0f) 
	{ 
		SetPosition(Vector2(-cameraPos - 445.0f, currentPos.y));
	}

	mPrevPos = GetPosition();
}

void Player::ActorInput(const uint8_t* keyState)
{

}