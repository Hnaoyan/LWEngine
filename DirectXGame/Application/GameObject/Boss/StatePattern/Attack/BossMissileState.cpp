#include "../StateMachine.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
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
	
	const int maxSize = 10;
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

		//---劣等---//
		// 右
		direct = LwLib::Slerp(rightSide[5], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 左
		direct = LwLib::Slerp(leftSide[5], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 右
		direct = LwLib::Slerp(rightSide[6], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);
		// 左
		direct = LwLib::Slerp(leftSide[6], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kInferior);

		//---優等---//
		// 右
		direct = LwLib::Slerp(rightSide[7], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);
		// 左
		direct = LwLib::Slerp(leftSide[7], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);



		//---優等---//
		// 右
		direct = LwLib::Slerp(rightSide[8], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);
		// 左
		direct = LwLib::Slerp(leftSide[8], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kSuperior);
		//---秀才---//
		// 右
		direct = LwLib::Slerp(rightSide[9], Vector3::Forward(), t);
		direct = Matrix4x4::TransformVector3(direct, rotateMatrix);
		GenerateMissile(direct, TrackingAttribute::kGenius);
		// 左
		direct = LwLib::Slerp(leftSide[9], Vector3::Forward(), t);
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
