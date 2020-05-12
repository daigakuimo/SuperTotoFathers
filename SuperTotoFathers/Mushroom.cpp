#include "Mushroom.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Texture.h"
#include "BoxComponent.h"
#include "CameraActor.h"
#include "MoveComponent.h"
#include "PhysWorld.h"

Mushroom::Mushroom(class Game* game)
	:Item(game)
	, moveSpeed(100.0f, 0.0f)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("../Assets/mushroom.png"));

	AABB myBox(Vector3(-32.0f, -32.0f, 0.f),
		Vector3(32.0f, 32.0f, 0.f));
	SetBoxComp(myBox);

	mc = new MoveComponent(this);
	mc->SetMass(1.0f);
	mc->SetVelocityLimit(Vector2(70.0f, 70.0f));
	mc->SetIsGravity(true);

	GetGame()->SetItem(this);
}

void Mushroom::UpdateActor(const float deltatime)
{
	GetBoxComp()->OnUpdateWorldTransform();
	// 地面にいるかどうか判定
	
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
	

	std::vector<class BoxComponent*> boxList = GetGame()->GetStageBoxes();

	for (auto box : boxList)
	{
		// 当たり判定
		Vector2 pos = GetPosition();
		const AABB& stageBox = box->GetWorldBox();
		const AABB& playerBox = GetBoxComp()->GetWorldBox();


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
			GetBoxComp()->OnUpdateWorldTransform();
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

void Mushroom::CollisionPlayer(class Actor* actor)
{
	actor->SetScale(2.0f);
	SetState(State::EDead);
}


