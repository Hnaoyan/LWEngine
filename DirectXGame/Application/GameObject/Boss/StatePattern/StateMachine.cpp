#include "StateMachine.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Attack/BossMissileBarrage.h"

void BossState::StateManager::Initialize(Boss* boss)
{
	boss_ = boss;
}

void BossState::StateManager::ChangeRequest(std::unique_ptr<IState> newState)
{
	if (boss_->GetState()) {
		// 終了処理
		boss_->GetState()->Exit();
	}

	newState->PreInitialize(boss_);
	newState->Initialize();

	boss_->ChangeState(std::move(newState));
}

void BossState::IState::PreInitialize(Boss* boss)
{
	// ポインタ設定
	boss_ = boss;

}

void BossState::IState::RotateUpdate()
{
	Vector3 normalize = boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition();
	normalize = Vector3::Normalize(normalize);
	normalize *= -1.0f;
	boss_->worldTransform_.transform_.rotate.y = std::atan2f(normalize.x, normalize.z);
}

void BossState::IState::TimerUpdate(StateVariant state)
{
	// 変更タイマー
	changeTimer_.Update();
	// 終了時に変更
	if (changeTimer_.IsEnd()/* || !changeTimer_.IsActive()*/) {
		boss_->GetDecider()->StateDecide(state);
		return;
	}
}

void BossState::IState::GenerateBullet(const Vector3& direct, const float& speed)
{
	// 生成部分
	BulletBuilder builder;
	builder.SetDirect(direct).SetSpeed(speed).SetTransform(boss_->worldTransform_.transform_);
	boss_->GetTrackingCluster()->AddBullet(builder, BulletType::kNormal);
}
