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
	cluster_ = boss_->GetBulletManager()->GetMissileCluster();
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
	DefaultMissile();


}

void BossState::MissileAttackState::DefaultMissile()
{
	// デフォルトの情報
	EulerTransform pos = boss_->worldTransform_.transform_;
	pos.scale = { bulletScale_,bulletScale_,bulletScale_ };
	// 行列の回転
	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateYMatrix(boss_->worldTransform_.transform_.rotate.y);
	rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);

#pragma region ランダム生成パターン
	Vector3 bossPosition = boss_->worldTransform_.GetWorldPosition();
	for (int i = 0; i < 25; ++i) {
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
		boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, direct, bulletSpeed_,
			boss_->GetPlayer(), BossSystemContext::TrackType::kStandard);
		//---劣等---//
		randomValue = LwLib::GetRandomValue(Vector3(-value, limitValue, limitValue), Vector3(value, value, value), limitValue);
		if (std::min(randomValue.z, 0.0f) == randomValue.z) {
			randomValue.z = 0.0f;
		}
		pos.translate = bossPosition + randomValue;
		direct = Vector3::Normalize(pos.translate - bossPosition);
		boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, direct, bulletSpeed_,
			boss_->GetPlayer(), BossSystemContext::TrackType::kInferior);
		//---優等---//
		randomValue = LwLib::GetRandomValue(Vector3(-value, limitValue, limitValue), Vector3(value, value, value), limitValue);
		if (std::min(randomValue.z, 0.0f) == randomValue.z) {
			randomValue.z = 0.0f;
		}
		pos.translate = bossPosition + randomValue;
		direct = Vector3::Normalize(pos.translate - bossPosition);
		boss_->GetBulletManager()->GetMissileCluster()->AddMissile(pos, direct, bulletSpeed_,
			boss_->GetPlayer(), BossSystemContext::TrackType::kSuperior);
	}
#pragma endregion

}
