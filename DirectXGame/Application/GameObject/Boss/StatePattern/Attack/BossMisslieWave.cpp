#include "BossMisslieWave.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BossState::MissileWaveState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	preActionTimer_.Start(30.0f);

	// 基底
	IState::Initialize();
}

void BossState::MissileWaveState::Update()
{
	// 回転
	RotateUpdate();
	// 待機部分の処理
	preActionTimer_.Update();
	spawnTimer_.Update();

	// ステートの稼働が開始するかどうか
	if (preActionTimer_.IsEnd()) {
		spawnTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackDuration"));
	}

	// 弾の生成部分
	if (spawnTimer_.IsEnd()) {

		// 終了なら
		if (spawnPhase_ == kEndPhase) {
			changeTimer_.Start(60.0f);
		}
		// でなければ
		else {
			// ミサイル生成
			SpawnMissile();
			// インクリメント
			spawnPhase_++;
			spawnTimer_.Start(GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackDuration"));
		}
	}

	// 終了用のタイマー更新
	TimerUpdate(this);
}

void BossState::MissileWaveState::Exit()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
}

void BossState::MissileWaveState::SpawnMissile()
{

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);
	Vector3 referenceLeftAxis = Vector3(-0.75f, 1.0f, -2.0f);
	Vector3 referenceRightAxis = Vector3(0.75f, 1.0f, -2.0f);
	TrackingAttribute attribute = TrackingAttribute::kSuperior;
	// 基準
	GenerateMissile(Matrix4x4::TransformVector3(referenceLeftAxis.Normalize(), rotateMatrix), attribute);
	GenerateMissile(Matrix4x4::TransformVector3(referenceRightAxis.Normalize(), rotateMatrix), attribute);
	// ずらし
	float offsetValue = 0.5f;
	//---左側---//
	Vector3 direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(offsetValue,0.0f,0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(-offsetValue, 0.0f, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(0.0f, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(0.0f, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(offsetValue, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(-offsetValue, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(offsetValue, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceLeftAxis + Vector3(-offsetValue, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	//---右側---//
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(offsetValue, 0.0f, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(-offsetValue, 0.0f, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(0.0f, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(0.0f, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(offsetValue, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(-offsetValue, offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(offsetValue, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
	direct = Matrix4x4::TransformVector3(Vector3::Normalize(referenceRightAxis + Vector3(-offsetValue, -offsetValue, 0.0f)), rotateMatrix);
	GenerateMissile(direct, attribute);
}

void BossState::MissileWaveState::GenerateMissile(const Vector3& direct, TrackingAttribute type)
{
	// デフォルトの情報
	EulerTransform transform = boss_->worldTransform_.transform_;

	BulletBuilder builder;
	float initSpeed = GlobalVariables::GetInstance()->GetValue<float>("BossAction", "WaveAttackInitSpeed");
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetTransform(transform).SetAttribute(type);
	builder.SetStraightFrame(GlobalVariables::GetInstance()->GetValue<float>("BossWaveAttack", "StFrame"));

	// 種類ごとに
	if (spawnPhase_ < kPhase3) {
		builder.SetAccuracyType(0);
		builder.SetAccelFrame(100.0f);

	}
	else if (spawnPhase_ < kPhase5) {
		builder.SetAccuracyType(1);
		builder.SetAccelFrame(80.0f);
		initSpeed *= 1.25f;
	}
	else {
		builder.SetAccuracyType(2);
		builder.SetAccelFrame(50.0f);
		initSpeed *= 1.6f;
	}

	builder.SetSpeed(initSpeed);

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
