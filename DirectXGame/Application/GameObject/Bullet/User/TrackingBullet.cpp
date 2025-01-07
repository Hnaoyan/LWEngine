#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwLibrary.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "../TrackingState/TrackingStates.h"
#include "../TrackingState/Transition/StateTransitionManager.h"
#include "Application/GameObject/Particle/User/ParticlePaths.h"

void TrackingBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	//if (object_->GetTag());
	Boss* boss = dynamic_cast<Boss*>(targetObject_);
	Player* player = dynamic_cast<Player*>(targetObject_);
	GlobalVariables* instance = GlobalVariables::GetInstance();
	// 追従対象がプレイヤー
	if (player) {
		// 
		data_.LoadGlobalData("BossTrackingBullet");
		// 直進の時間設定
		//straightFrame_ = instance->GetValue<float>("BossTrackingBullet", "StraightFrame");
		isTargetBoss_ = false;
	}
	// 追従対象がボス
	else if (boss) {
		// 
		data_.LoadGlobalData("PlayerTrackingBullet");
		// 直進の時間設定
		straightFrame_ = (instance->GetValue<float>("PlayerTrackingBullet", "StraightFrame"));
		// 対象がボスか
		isTargetBoss_ = true;
	}

	// 速度を距離に応じて
	float reductionRate = 0.0f;	// 減速するレート
	float reductionDistance = 175.0f;	// 制限距離
	float redMinDistance = 100.0f;
	float toDistance = TargetToDistance();
	if (TargetToDistance() <= reductionDistance) {
		float v = LwLib::Normalize(toDistance, redMinDistance, reductionDistance);
		reductionRate = Ease::Easing(0.65f, 1.0f, v);
		//data_.initSpeed *= reductionRate;
		//data_.baseSpeed *= reductionRate;
		velocity_ *= reductionRate;
	}
	// 初期の向き
	defaultTargetDirection_ = Vector3::Normalize(targetObject_->worldTransform_.GetWorldPosition() - GetWorldPosition());

	// ステートの設定
	stateMachine_ = std::make_unique<BulletStateMachine>(this);
	stateMachine_->RequestState(TrackingState::kStraight);
	// ステートの遷移クラス
	transitionManager_ = std::make_unique<StateTransitionManager>();
	transitionManager_->Initialize(this);
}

void TrackingBullet::Update()
{
	// 遷移クラス
	transitionManager_->Update();

	//ChangeSelecter();

	// ステートの処理
	stateMachine_->Update(true);

	// 移動
	velocity_ += accelerate_ * GameSystem::GameSpeedFactor();
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor();
	transform_.rotate.y += GameSystem::GameSpeedFactor();

	// 基底の更新
	IBullet::Update();
}

void TrackingBullet::ImGuiDraw()
{

}

void TrackingBullet::OnCollision(ColliderObject object)
{
	// 地形
	if (std::holds_alternative<Terrain*>(object)) {
		isDead_ = true;
	}
	// プレイヤー
	if (std::holds_alternative<Player*>(object)) {
		isDead_ = true;
	}
	// 
	if (std::holds_alternative<Boss*>(object)) {
		isDead_ = true;
	}
}
