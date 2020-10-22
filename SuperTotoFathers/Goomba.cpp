#include "Goomba.h"
#include "Texture.h"
#include "AnimSpriteComponent.h"
#include "MoveComponent.h"
#include "BoxComponent.h"
#include "CircleComponent.h"
#include "PhysWorld.h"
#include "CameraActor.h"


Goomba::Goomba(Game* game)
	:Actor(game)
	,moveSpeed(-100.0f,0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<class Texture*> anims = {
		game->GetTexture("Assets/Goomba-1.png"),
		game->GetTexture("Assets/Goomba-2.png"),
		game->GetTexture("Assets/Goomba-3.png"),
	};

	asc->SetAnimTextures(anims);
	asc->SetAnimRange(ActionState::EStop, 0, 0, true);
	asc->SetAnimRange(ActionState::EDeath, 2, 2, true);
	asc->SetAnimRange(ActionState::EWalk, 0, 1, true);
	asc->SetAnimRange(ActionState::EJump, 0, 0, true);
	asc->SetAnimRange(ActionState::EFall, 0, 0, true);
	asc->SetAnimFPS(5.0f);

	SetActionState(ActionState::EWalk);

	mc = new MoveComponent(this);
	mc->SetMass(1.0f);
	mc->SetVelocityLimit(Vector2(70.0f, 70.0f));
	mc->SetIsGravity(true);

	mBoxComp = new BoxComponent(this);

	AABB myBox(Vector3(-32.0f, -32.0f, 0.f),
		Vector3(32.0f, 32.0f, 0.f));
	mBoxComp->SetObjectBox(myBox);
	mBoxComp->SetShouldRotate(false);

	Sphere myCircle(Vector3(GetPosition().x, GetPosition().y, 0.0f), 32.0f);
	mCircleComp = new CircleComponent(this);
	mCircleComp->SetObjectCircle(myCircle);

	game->SetEnemys(mCircleComp);
}

Goomba::~Goomba()
{
	GetGame()->RemoveEnemy(mCircleComp);
}

void Goomba::UpdateActor(float deltaTime)
{
	mBoxComp->OnUpdateWorldTransform();
	// 地面にいるかどうか判定
	if (GetActionState() != ActionState::EDeath)
	{
		Vector3 start = Vector3(GetPosition().x, GetPosition().y, 0);
		Vector3 dir = Vector3(0, -1, 0);
		Vector3 end = start + dir * mDOWN_SEGMENT_LENGTH;
		LineSegment downLs(start, end);

		PhysWorld* phys = GetGame()->GetPhysWorld();
		PhysWorld::CollisionInfo info;
		if (phys->AvoidActorSegmentCast(downLs, info, this))
		{
			SetActionState(ActionState::EWalk);
			mc->AddForce(moveSpeed);
		}
		else
		{
			SetActionState(ActionState::EJump);
		}
	}
	else
	{
		deathCount++;

		if (deathCount >= 50)
		{
			SetState(State::EDead);
		}
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
				if (Math::Abs(dx1) < Math::Abs(dx2))
				{
					moveSpeed = Vector2(-100.0f, 0.f);
				}
				else
				{
					moveSpeed = Vector2(100.0f, 0.f);
				}
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

	// 画面の画面外に出たらインスタンスを削除
	float cameraPos = GetGame()->GetCameraActor()->GetBeforeX();
	Vector2 currentPos = GetPosition();
	if (currentPos.x < -cameraPos - 573.0f)
	{
		SetState(State::EDead);
	}
}

void Goomba::ActorInput(const uint8_t* keyState)
{

}
