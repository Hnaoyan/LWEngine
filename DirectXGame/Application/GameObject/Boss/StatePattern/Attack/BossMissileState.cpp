#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include <algorithm>

void BossState::MissileAttackState::Initialize()
{
	GlobalVariables* global = GlobalVariables::GetInstance();

	boss_->SetNowVariantState(this);
	// 開くアニメーションの受付
	boss_->GetAnimManager()->AnimationExecute(AnimType::kOpen, global->GetValue<float>("BossAnimation", "OpenFrame"));

	// アクション前の待機タイマー
	preActionTimer_.Start(60.0f);

	// 
	IState::Initialize();
}

void BossState::MissileAttackState::Update()
{
	// 
	preActionTimer_.Update();
	// 待機時間終了時
	if (preActionTimer_.IsEnd()) {
		float changeFrame = 120.0f;
		// 変更までの時間
		changeTimer_.Start(changeFrame);
		// 回転の処理
		RotateUpdate();
		//---弾の情報---//
		// 進む方向
		bulletDirect_ = Vector3::Normalize(boss_->GetPlayer()->worldTransform_.GetWorldPosition() - boss_->worldTransform_.GetWorldPosition());
		MissileAttack();
	}
	attackTimer_.Update();
	TimerUpdate(this);

	if (attackTimer_.IsEnd()) {
		float attackDuration = 15.0f;
		attackTimer_.Start(attackDuration);
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
	
	const int maxSize = 5;
	std::vector<Vector3> rightSide;
	std::vector<Vector3> leftSide;
	rightSide.resize(maxSize);
	leftSide.resize(maxSize);
	for (int xy = 0; xy < maxSize; ++xy) {
		rightSide[xy] = LwLib::Slerp(Vector3(0.1f, 1.0f, 0.0f), Vector3::Right(), (float)(xy + 1) / maxSize);
		leftSide[xy] = LwLib::Slerp(Vector3(-0.1f, 1.0f, 0.0f), Vector3::Left(), (float)(xy + 1) / maxSize);
	}
	for (int yz = 0; yz < 5; ++yz) {
		float maxScale = 0.5f;	// 最大値
		float t = ((float)1 / (yz + 1)) * maxScale;	// LerpT
		// 方向
		Vector3 direct{};
#pragma region Phase1
		//---秀才---//
		// 右
		direct = LwLib::Slerp(rightSide[0], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kGenius);
		// 左
		direct = LwLib::Slerp(leftSide[0], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kGenius);

		//---劣等---//
		// 右
		direct = LwLib::Slerp(rightSide[1], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 左
		direct = LwLib::Slerp(leftSide[1], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 右
		direct = LwLib::Slerp(rightSide[2], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 左
		direct = LwLib::Slerp(leftSide[2], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);

		//---優等---//
		// 右
		direct = LwLib::Slerp(rightSide[3], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);
		// 左
		direct = LwLib::Slerp(leftSide[3], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);
#pragma endregion

		//---秀才---//
		// 右
		direct = LwLib::Slerp(rightSide[4], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kGenius);
		// 左
		direct = LwLib::Slerp(leftSide[4], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kGenius);
	}
}

void BossState::MissileAttackState::GenerateMissile(const Matrix4x4& rotateMatrix, TrackingAttribute type)
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
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "InitSpeed")).SetTransform(transform).SetAttribute(type);
	
	// 直進の時間
	float straightTime = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame");
	straightTime = LwLib::GetRandomValue(straightTime, straightTime + 60.0f);
	builder.SetStraightFrame(straightTime);

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

	//boss_->GetTrackingCluster()->AddBullet(builder, BulletType::kTracking);

}

void BossState::MissileAttackState::GenerateMissile(const Vector3& direct, TrackingAttribute type)
{
	// デフォルトの情報
	EulerTransform transform = boss_->worldTransform_.transform_;

	BulletBuilder builder;
	builder.SetTargetObject(boss_->GetPlayer()).SetDirect(direct).SetSpeed(GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "InitSpeed")).SetTransform(transform).SetAttribute(type);
	//float minFrame = GlobalVariables::GetInstance()->GetValue<float>("BossTrackingBullet", "StraightFrame");
	float minFrame = 0;
	float maxFrame = minFrame + 200.0f;
	builder.SetStraightFrame(LwLib::GetRandomValue(minFrame, maxFrame));
	
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
