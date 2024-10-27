#include "BossMissileBarrage.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"

void BossState::MissileBarrageState::Initialize()
{
	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, 55.0f);

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);
}

void BossState::MissileBarrageState::Update()
{
	// 攻撃の処理
	attackTimer_.Update();
	if (attackTimer_.IsEnd()) {
		attackTimer_.Start(15.0f);
		// 攻撃処理
		Attack();

	}
	// 待機部分の処理
	preActionTimer_.Update();
	if (preActionTimer_.IsEnd()) {
		// 変更までの時間
		changeTimer_.Start(120.0f);
		attackTimer_.Start(15.0f);
		// 攻撃処理
		Attack();
	}

	TimerUpdate(this);
}

void BossState::MissileBarrageState::Exit()
{
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, 30.0f);
}

void BossState::MissileBarrageState::Attack()
{
	// 回転
	RotateUpdate();

	// ベクトル
	Vector3 upVector = Vector3::Up();
	Vector3 rightVector = Vector3::Right();
	Vector3 leftVector = rightVector * (-1.0f);
	Vector3 rightUp = Vector3(1.0f, 1.0f, 0.0f);
	Vector3 leftUp = Vector3(-1.0f, 1.0f, 0.0f);

	upVector = LwLib::Slerp(upVector, Vector3::Backward(), changeTimer_.GetElapsedFrame());
	rightVector = LwLib::Slerp(rightVector, Vector3::Backward(), changeTimer_.GetElapsedFrame());
	leftVector = LwLib::Slerp(leftVector, Vector3::Backward(), changeTimer_.GetElapsedFrame());
	rightUp = LwLib::Slerp(rightUp, Vector3::Backward(), changeTimer_.GetElapsedFrame());
	leftUp = LwLib::Slerp(leftUp, Vector3::Backward(), changeTimer_.GetElapsedFrame());

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);

	// 上
	GenerateMissile(Matrix4x4::TransformVector3(upVector, rotateMatrix), TrackingType::kStandard);

	// 右
	GenerateMissile(Matrix4x4::TransformVector3(rightVector, rotateMatrix), TrackingType::kStandard);
	// 右上
	GenerateMissile(Matrix4x4::TransformVector3(rightUp, rotateMatrix), TrackingType::kStandard);

	// 左
	GenerateMissile(Matrix4x4::TransformVector3(leftVector, rotateMatrix), TrackingType::kStandard);
	// 左上
	GenerateMissile(Matrix4x4::TransformVector3(leftUp, rotateMatrix), TrackingType::kStandard);
}

void BossState::MissileBarrageState::GenerateMissile(const Vector3& direct, TrackingType type)
{
	EulerTransform transform = boss_->worldTransform_.transform_;
	float offsetValue = 2.0f;
	transform.translate += Vector3::Normalize(direct) * offsetValue;

	// インスタンスを生成から送るまで
	std::unique_ptr<IBullet> bullet = std::make_unique<TrackingBullet>();
	static_cast<TrackingBullet*>(bullet.get())->SetGameObject(boss_->GetPlayer());
	static_cast<TrackingBullet*>(bullet.get())->SetTrackType(type);
	bullet->Initialize();
	bullet->SetVelocity(direct * TrackingBullet::sInitSpeed);
	bullet->transform_ = transform;
	bullet->transform_.scale = GlobalVariables::GetInstance()->GetValue<Vector3>("BossNormalBullet", "Scale");
	boss_->GetTrackingCluster()->AddBullet(std::move(bullet));
}
