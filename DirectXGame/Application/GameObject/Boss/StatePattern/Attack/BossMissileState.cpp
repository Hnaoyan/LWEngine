#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include <algorithm>

uint32_t BossState::MissileAttackState::sMissileClusterSerial = 0;

void BossState::MissileAttackState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);
	// クラスター
	//cluster_ = boss_->GetBulletManager()->GetMissileCluster();
	clusterSerial = sMissileClusterSerial;
	sMissileClusterSerial++;
}

void BossState::MissileAttackState::Update()
{
	// 
	preActionTimer_.Update();
	// 待機時間終了時
	if (preActionTimer_.IsEnd()) {
		// 変更までの時間
		changeTimer_.Start(120.0f);
		// 回転の処理
		RotateUpdate();
		//---弾の情報---//
		// 進む方向
		bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		MissileAttack();
		//attackTimer_.Start(15.0f);
	}
	attackTimer_.Update();
	TimerUpdate(this);

	if (attackTimer_.IsEnd()) {
		attackTimer_.Start(15.0f);
		// 回転の処理
		RotateUpdate();
		//---弾の情報---//
		// 進む方向
		bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		MissileAttack();
	}
}

void BossState::MissileAttackState::Exit()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
}

void BossState::MissileAttackState::MissileAttack()
{
	// 行列の回転
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);
	// 弾の生成
	for (int i = 0; i < 25; ++i) {
		//---優等---//
		GenerateMissile(rotateMatrix, TrackingType::kSuperior);
		//---劣等---//
		GenerateMissile(rotateMatrix, TrackingType::kInferior);
		GenerateMissile(rotateMatrix, TrackingType::kInferior);
		GenerateMissile(rotateMatrix, TrackingType::kInferior);
		//---秀才---//
		GenerateMissile(rotateMatrix, TrackingType::kGenius);
		//GenerateMissile(rotateMatrix, TrackingType::kGenius);
	}
}

void BossState::MissileAttackState::GenerateMissile(const Matrix4x4& rotateMatrix, TrackingType type)
{
	// デフォルトの情報
	EulerTransform transform = boss_->worldTransform_.transform_;
	Vector3 bossPosition = boss_->worldTransform_.GetWorldPosition();

	float value = 5.0f;
	float limitValue = 1.5f;
	//---通常---//
	Vector3 randomValue = LwLib::GetRandomValue(Vector3(-value, limitValue, limitValue), Vector3(value, value, value), limitValue);
	if (std::min(randomValue.z, 0.0f) == randomValue.z) {
		randomValue.z = 0.0f;
	}
	transform.translate = bossPosition + randomValue;
	Vector3 direct = Vector3::Normalize(transform.translate - bossPosition);
	direct = Matrix4x4::TransformVector3(direct, rotateMatrix);

	BulletBuilder builder;
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetSpeed(TrackingBullet::sInitSpeed).SetTransform(transform).SetType(type);
	
	boss_->GetTrackingCluster()->AddBullet(builder, BulletType::kTracking);

}
