#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"

void BossState::UpDownState::Initialize()
{
	boss_->SetNowVariantState(this);

	// 最大・最小
	float minY = GlobalVariables::GetInstance()->GetValue<float>("Boss", "MinYPoint");
	float maxY = GlobalVariables::GetInstance()->GetValue<float>("Boss", "MaxYPoint");

	if (boss_->worldTransform_.GetWorldPosition().y > minY) {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y = minY;
	}
	else {
		startPosition_ = boss_->worldTransform_.GetWorldPosition();
		endPosition_ = startPosition_;
		endPosition_.y = maxY;
	}
	// 変更のタイマー
	float hpThreshold = 1.0f / 2.0f;
	if (boss_->GetHealth()->GetHPRatio() > hpThreshold) {
		changeTimer_.Start(65.0f);
	}
	else {
		changeTimer_.Start(40.0f);
	}

}

void BossState::UpDownState::Update()
{
	// 移動処理
	boss_->worldTransform_.transform_.translate = Vector3::Lerp(startPosition_, endPosition_, changeTimer_.GetElapsedFrame());
	TimerUpdate(this);
}

void BossState::UpDownState::Exit()
{
	boss_->SetPrevVariantState(this);
}
