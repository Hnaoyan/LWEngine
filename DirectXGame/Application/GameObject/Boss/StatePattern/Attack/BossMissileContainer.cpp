#include "BossMissileContainer.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BossState::MissileContainerState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);

}

void BossState::MissileContainerState::Update()
{
	// 回転
	RotateUpdate();
	// 待機部分の処理
	preActionTimer_.Update();
	spawnTimer_.Update();

	// 開始と同時に
	if (preActionTimer_.IsEnd()) {
		SpawnMissile();
		spawnTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackDuration"));
	}

	// 変更タイマースタート判定
	if (spawnTimer_.IsEnd()) {
		SpawnMissile();
		changeTimer_.Start(60.0f);
	}

	// 終了用のタイマー更新
	TimerUpdate(this);
}

void BossState::MissileContainerState::Exit()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
}

void BossState::MissileContainerState::SpawnMissile()
{
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);
	Vector3 referenceLeftAxis = Vector3(-1.0f, 1.0f, 1.0f);
	Vector3 referenceRightAxis = Vector3(1.0f, 1.0f, 1.0f);
	TrackingAttribute attribute = TrackingAttribute::kGenius;

	GenerateMissile(Matrix4x4::TransformVector3(referenceLeftAxis.Normalize(),rotateMatrix), attribute);
	GenerateMissile(Matrix4x4::TransformVector3(referenceRightAxis.Normalize(), rotateMatrix), attribute);
}

void BossState::MissileContainerState::GenerateMissile(const Vector3& direct, TrackingAttribute type)
{
	// デフォルトの情報
	EulerTransform transform = boss_->worldTransform_.transform_;
	transform.scale *= 1.5f;

	// ビルダー
	BulletBuilder builder;
	builder.SetTargetObject(boss_).SetDirect(direct).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackInitSpeed")).SetTransform(transform).SetAttribute(type);
	builder.SetAttribute(type).SetParentAttribute(0);
	// Clusterのタイプ設定
	switch (type)
	{
	case TrackingAttribute::kSuperior:
		builder.SetCluster(boss_->GetSuperiorCluster());
		break;
	case TrackingAttribute::kInferior:
		builder.SetCluster(boss_->GetInferiorCluster());
		break;
	case TrackingAttribute::kGenius:
		builder.SetCluster(boss_->GetGeneusCluster());
		break;
	case TrackingAttribute::kNone:
		break;
	case TrackingAttribute::kMaxSize:
		break;
	}

	boss_->GetContainerCluster()->AddBullet(builder, BulletType::kContainer);
}
