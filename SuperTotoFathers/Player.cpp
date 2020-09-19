#include "Player.h"
#include "Game.h"
#include "Texture.h"
#include "InputComponent.h"
#include "AnimSpriteComponent.h"
#include "CameraActor.h"
#include "BoxComponent.h"
#include "CircleComponent.h"
#include "PhysWorld.h"
#include "Brock.h"
#include "Item.h"
#include "Goal.h"
#include "AudioSystem.h"
#include "AudioComponent.h"

Player::Player(Game* game)
	:Actor(game)
{
	
	// Create an animation sprite compoent
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<class Texture*> anims = {
		game->GetTexture("../Assets/Toto1-1.png"),
		game->GetTexture("../Assets/Toto1-2.png"),
		game->GetTexture("../Assets/Toto1-3.png"),
		game->GetTexture("../Assets/Toto1-4.png"),
		game->GetTexture("../Assets/Toto1-5.png"),
		game->GetTexture("../Assets/Toto1-6.png"),
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimRange(ActionState::EStop, 0, 0, true);
	asc->SetAnimRange(ActionState::EDeath, 4, 4, true);
	asc->SetAnimRange(ActionState::EWalk, 1, 2, true);
	asc->SetAnimRange(ActionState::EJump, 3, 3, true);
	asc->SetAnimRange(ActionState::EFall, 0, 0, true);
	asc->SetAnimRange(ActionState::EGoal, 5, 5, true);

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

	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);

}

void Player::UpdateActor(float deltaTime)
{
	mBoxComp->OnUpdateWorldTransform();
	// 地面にいるかどうか判定
	Vector3 start = Vector3(GetPosition().x, GetPosition().y, 0);
	Vector3 dir = Vector3(0, -1, 0);
	Vector3 end = start + dir * mDOWN_SEGMENT_LENGTH * GetScale();
	LineSegment downLs(start, end);

	PhysWorld* phys = GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;

	if (ActionState::EDeath != GetActionState())
	{
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

		// アイテムとの当たり判定
		std::vector<class Item*> itemList = GetGame()->GetItems();
		for (auto item : itemList)
		{
			// 当たり判定
			Vector2 pos = GetPosition();

			const AABB& itemBox = item->GetBoxComp()->GetWorldBox();
			const AABB& playerBox = mBoxComp->GetWorldBox();

			if (Intersect(playerBox, itemBox))
			{
				item->CollisionPlayer(this);

				// 位置を調整
				mBoxComp->OnUpdateWorldTransform();
			}
		}

		// ステージ(壊れないオブジェクト)との当たり判定
		std::vector<class BoxComponent*> boxList = GetGame()->GetStageBoxes();
		for (auto box : boxList)
		{
			// 当たり判定
			Vector2 pos = GetPosition();

			const AABB& stageBox = box->GetWorldBox();
			const AABB& playerBox = mBoxComp->GetWorldBox();

			if (Intersect(playerBox, stageBox))
			{
				// 全ての差を計算する
				float dx1 = stageBox.mMin.x - playerBox.mMax.x;
				float dx2 = stageBox.mMax.x - playerBox.mMin.x;
				float dy1 = stageBox.mMin.y - playerBox.mMax.y;
				float dy2 = stageBox.mMax.y - playerBox.mMin.y;

				// 絶対値の小さい方をセットする
				float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
				float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

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


		// ブロックとの当たり判定
		std::vector<class Brock*> brockList = GetGame()->GetBrocks();
		for (auto brock : brockList)
		{
			// 当たり判定
			Vector2 pos = GetPosition();

			const AABB& brockBox = brock->GetBoxComp()->GetWorldBox();
			const AABB& playerBox = mBoxComp->GetWorldBox();

			if (Intersect(playerBox, brockBox))
			{
				// 全ての差を計算する
				float dx1 = brockBox.mMin.x - playerBox.mMax.x;
				float dx2 = brockBox.mMax.x - playerBox.mMin.x;
				float dy1 = brockBox.mMin.y - playerBox.mMax.y;
				float dy2 = brockBox.mMax.y - playerBox.mMin.y;

				// 絶対値の小さい方をセットする
				float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
				float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

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

				// ジャンプ中に下からブロックに当たったら、ブロックを潰す
				if ((brockBox.mMin.y >= playerBox.mMax.y) && (ic->GetForwardSpeed().y > 0))
				{
					ic->SetForwardSpeed(Vector2(ic->GetForwardSpeed().x, 0.f));
					ic->SetJumpPower(-30.0f);
					brock->SetActionState(ActionState::EDeath);
				}
			}
		}


		// 敵との当たり判定
		std::vector<class CircleComponent*> enemyList = GetGame()->GetEnemys();
		for (auto enemy : enemyList)
		{
			const AABB& playerBox = mBoxComp->GetWorldBox();
			const Sphere& enemyCircle = enemy->GetWorldCircle();
			if (enemy->GetOwner()->GetActionState() == ActionState::EDeath)
			{
				continue;
			}
			if (Intersect(enemyCircle, playerBox))
			{
				float hitPos = Math::Max(playerBox.mMin.y - enemyCircle.mCenter.y, 0.0f);
				hitPos = Math::Max(hitPos, enemyCircle.mCenter.y - playerBox.mMax.y);
				if (hitPos == playerBox.mMin.y - enemyCircle.mCenter.y)
				{
					ic->SetForwardSpeed(Vector2(ic->GetForwardSpeed().x, 0.f));
					ic->SetJumpPower(60.0f);
					enemy->GetOwner()->SetActionState(ActionState::EDeath);
				}
				else
				{
					SetActionState(ActionState::EDeath);
					ic->SetJumpPower(100.0f);
				}
			}
		}

		// ゴールとの当たり判定
		class Goal* goal = GetGame()->GetGoal();
		const AABB& playerBox = mBoxComp->GetWorldBox();
		const AABB& goalBox = goal->GetBoxComp()->GetWorldBox();
		if (Intersect(goalBox, playerBox))
		{
			// 当たり判定
			Vector2 pos = GetPosition();

			// 全ての差を計算する
			float dx1 = goalBox.mMin.x - playerBox.mMax.x;
			float dx2 = goalBox.mMax.x - playerBox.mMin.x;
			float dy1 = goalBox.mMin.y - playerBox.mMax.y;
			float dy2 = goalBox.mMax.y - playerBox.mMin.y;

			// 絶対値の小さい方をセットする
			float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
			float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

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

			SetActionState(ActionState::EGoal);
			goal->SetActionState(ActionState::EWalk);
		}

	}

	// Play the footstep if we're moving and haven't recently
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(ic->GetForwardSpeed().x) && mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
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