#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::TeleportState::Initialize()
{
	boss_->SetNowVariantState(this);
	//boss_->worldTransform_.transform_.translate.y = 20.0f;
	boss_->worldTransform_.UpdateMatrix();

	changeTimer_.Start(150.0f);

	if (isCross_) {
		isCross_ = false;
	}
	else {
		isCross_ = true;
	}
	startPosition_ = boss_->worldTransform_.GetWorldPosition();
}

void BossState::TeleportState::Update()
{

	boss_->worldTransform_.transform_.translate = Vector3::Lerp(startPosition_, endPosition_, changeTimer_.GetElapsedFrame());

	TimerUpdate(this);
}

void BossState::TeleportState::Exit()
{
	boss_->SetPrevVariantState(this);
}

void BossState::TeleportState::SelectTeleportPoint(const Vector3& position)
{
	float yPosition = 20.0f;
	std::vector<Vector3> points;
	// 右奥
	points.push_back({ kMaxPoint,yPosition,kMaxPoint });
	// 左奥
	points.push_back({ -kMaxPoint,yPosition,kMaxPoint });
	// 右手前
	points.push_back({ kMaxPoint,yPosition,-kMaxPoint });
	// 左手前
	points.push_back({ -kMaxPoint,yPosition,-kMaxPoint });

	if (isCross_) {
		// 右前
		if (position.x > 0 && position.z > 0) {
			endPosition_ = points[3];
		}
		// 左前
		else if (position.x <= 0 && position.z > 0) {
			endPosition_ = points[2];
		}
		// 右後ろ
		else if (position.x > 0 && position.z <= 0) {
			endPosition_ = points[1];
		}
		// 左後ろ
		else if (position.x <= 0 && position.z <= 0) {
			endPosition_ = points[0];
		}
	}
}
