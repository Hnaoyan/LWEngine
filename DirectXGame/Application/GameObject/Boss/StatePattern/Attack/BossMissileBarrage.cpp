#include "BossMissileBarrage.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void BossState::MissileBarrageState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);
}

void BossState::MissileBarrageState::Update()
{
	// 攻撃の処理
	attackTimer_.Update();
	if (attackTimer_.IsEnd()) {
		attackTimer_.Start(5.0f);
		// 攻撃処理
		Attack();

	}
	// 待機部分の処理
	preActionTimer_.Update();
	if (preActionTimer_.IsEnd()) {
		// 変更までの時間
		changeTimer_.Start(120.0f);
		attackTimer_.Start(5.0f);
		// 攻撃処理
		Attack();
	}

	TimerUpdate(this);
}

void BossState::MissileBarrageState::Exit()
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	boss_->SetPrevVariantState(this);
	boss_->GetAnimManager()->AnimationExecute(AnimType::kClose, global->GetValue<float>("BossAnimation", "CloseFrame"));
}

void BossState::MissileBarrageState::Attack()
{
	// 回転
	RotateUpdate();

	// ここがコンテナの役割
	// 始点ベクトルー終点ベクトルでSlerpした方向に飛ばす
	// ベクトル
	Vector3 upVector = Vector3::Up();			// 上
	Vector3 rightVector = Vector3::Right();		// 右
	Vector3 leftVector = Vector3::Left();	// 左
	Vector3 rightUp = Vector3::Up() + Vector3::Right();	// 右上
	Vector3 leftUp = Vector3::Up() + Vector3::Left();	// 左上

	float t = std::clamp(changeTimer_.GetElapsedFrame(), 0.0f, 1.0f);

	// 上
	Vector3 start = LwLib::Slerp(Vector3::Backward(), upVector, 0.3f);
	upVector = LwLib::Slerp(start, upVector, t);

	// 右
	start = LwLib::Slerp(Vector3::Backward(), rightVector, 0.3f);
	rightVector = LwLib::Slerp(start, rightVector, t);

	// 左
	start = LwLib::Slerp(Vector3::Backward(), leftVector, 0.3f);
	leftVector = LwLib::Slerp(start, leftVector, t);

	// 右上
	start = LwLib::Slerp(Vector3::Backward(), rightUp, 0.3f);
	rightUp = LwLib::Slerp(start, rightUp, t);

	// 左上
	start = LwLib::Slerp(Vector3::Backward(), leftUp, 0.3f);
	leftUp = LwLib::Slerp(start, leftUp, t);

	Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateXYZMatrix(boss_->worldTransform_.transform_.rotate);

	// 上
	GenerateMissile(Matrix4x4::TransformVector3(upVector, rotateMatrix), TrackingAttribute::kSuperior);

	// 右
	GenerateMissile(Matrix4x4::TransformVector3(rightVector, rotateMatrix), TrackingAttribute::kSuperior);
	// 右上
	GenerateMissile(Matrix4x4::TransformVector3(rightUp, rotateMatrix), TrackingAttribute::kSuperior);

	// 左
	GenerateMissile(Matrix4x4::TransformVector3(leftVector, rotateMatrix), TrackingAttribute::kSuperior);
	// 左上
	GenerateMissile(Matrix4x4::TransformVector3(leftUp, rotateMatrix), TrackingAttribute::kSuperior);
}

void BossState::MissileBarrageState::GenerateMissile(const Vector3& direct, TrackingAttribute type)
{
	// 設定部分
	EulerTransform transform = boss_->worldTransform_.transform_;
	float offsetValue = 2.0f;
	transform.translate += Vector3::Normalize(direct) * offsetValue;

	// 生成部分
	BulletBuilder builder;
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetSpeed(TrackingBullet::sInitSpeed).SetTransform(transform).SetType(type);
	boss_->GetTrackingCluster()->AddBullet(builder, BulletType::kTracking);
}
