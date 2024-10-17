#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include <algorithm>

void BossState::MissileAttackState::Initialize()
{
	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, 55.0f);

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);
	// クラスター
	//cluster_ = boss_->GetBulletManager()->GetMissileCluster();
}

void BossState::MissileAttackState::Update()
{
	// 
	preActionTimer_.Update();
	// 待機時間終了時
	if (preActionTimer_.IsEnd()) {
		// 変更までの時間
		changeTimer_.Start(120.0f);
		RotateUpdate();
		//---弾の情報---//
		// 速さ
		bulletSpeed_ = BossSystemContext::TrackingBullet::sInitSpeed;
		// サイズ
		bulletScale_ = 0.75f;
		// 進む方向
		bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		MissileAttack();
	}
	TimerUpdate(this);
}

void BossState::MissileAttackState::Exit()
{
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, 30.0f);
}

void BossState::MissileAttackState::MissileAttack()
{
	// 行列の回転
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);
	// 弾の生成
	for (int i = 0; i < 25; ++i) {
		//---通常---//
		GenerateMissile(rotateMatrix, BossSystemContext::TrackType::kStandard);
		//---劣等---//
		GenerateMissile(rotateMatrix, BossSystemContext::TrackType::kInferior);
		//---優等---//
		GenerateMissile(rotateMatrix, BossSystemContext::TrackType::kSuperior);
	}
}

void BossState::MissileAttackState::GenerateMissile(const Matrix4x4& rotateMatrix, BossSystemContext::TrackType type)
{
	// デフォルトの情報
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	Vector3 bossPosition = boss_->worldTransform_.GetWorldPosition();

	float value = 5.0f;
	float limitValue = 1.5f;
	//---通常---//
	Vector3 randomValue = LwLib::GetRandomValue(Vector3(-value, limitValue, limitValue), Vector3(value, value, value), limitValue);
	if (std::min(randomValue.z, 0.0f) == randomValue.z) {
		randomValue.z = 0.0f;
	}
	pos.translate = bossPosition + randomValue;
	Vector3 direct = Vector3::Normalize(pos.translate - bossPosition);
	direct = Matrix4x4::TransformVector3(direct, rotateMatrix);

	// インスタンスを生成から送るまで
	std::unique_ptr<IBullet> bullet = std::make_unique<BossSystemContext::TrackingBullet>();
	static_cast<BossSystemContext::TrackingBullet*>(bullet.get())->SetPlayer(boss_->GetPlayer());
	static_cast<BossSystemContext::TrackingBullet*>(bullet.get())->SetTrackType(type);
	bullet->Initialize();
	bullet->SetVelocity(direct * bulletSpeed_);
	bullet->transform_ = pos;
	bullet->transform_.scale = GlobalVariables::GetInstance()->GetValue<Vector3>("BossNormalBullet", "Scale");
	boss_->GetTrackingCluster()->AddBullet(std::move(bullet));

}
