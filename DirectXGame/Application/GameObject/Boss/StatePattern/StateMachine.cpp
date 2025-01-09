#include "StateMachine.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Attack/BossMissileBarrage.h"
#include "Attack/BossMisslieWave.h"
#include "Attack/BossMissileContainer.h"

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

void BossState::IState::Initialize()
{
	// 初期化
	bulletComponent_.Initialize(boss_);
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

void BossState::BulletComponent::Initialize(Boss* boss)
{
	assert(boss);
	boss_ = boss;
}

void BossState::BulletComponent::GenerateTracking(const Vector3& direct, TrackingAttribute type)
{
	// デフォルトの情報
	EulerTransform transform = boss_->worldTransform_.transform_;

	BulletBuilder builder;
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackInitSpeed")).SetTransform(transform).SetAttribute(type);
	builder.SetStraightFrame(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame"));

	switch (type)
	{
	case TrackingAttribute::kSuperior:
		boss_->GetSuperiorCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	case TrackingAttribute::kInferior:
		boss_->GetInferiorCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	case TrackingAttribute::kGenius:
		boss_->GetGeneusCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	}
}

void BossState::BulletComponent::Fire(BulletBuilder builder, TrackingAttribute type)
{
	switch (type)
	{
	case TrackingAttribute::kSuperior:
		boss_->GetSuperiorCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	case TrackingAttribute::kInferior:
		boss_->GetInferiorCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	case TrackingAttribute::kGenius:
		boss_->GetGeneusCluster()->AddBullet(builder, BulletType::kTracking);
		break;
	case TrackingAttribute::kNone:
		break;
	case TrackingAttribute::kMaxSize:
		break;
	default:
		break;
	}
}
