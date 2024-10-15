#include "TrackingBullet.h"
#include "Application/Collision/ColliderFilter.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameObject/GameObjectLists.h"
#include "Engine/LwLib/LwEngineLib.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void TrackingBullet::Initialize()
{
	// 基底の初期化・コライダーのマスク設定
	IBullet::Initialize();
	collider_.SetAttribute(kCollisionAttributeEnemyBullet);

	GlobalVariables* instance = GlobalVariables::GetInstance();

	// 軌跡
	trail_->SetLength(50);
	trail_->polygon_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	trail_->polygon_->SetWidth(instance->GetValue<float>("BossTrackingBullet", "TrailMaxWidth"));
	trail_->polygon_->SetMinWidth(instance->GetValue<float>("BossTrackingBullet", "TrailMinWidth"));
}

void TrackingBullet::Update()
{
	//// 追尾タイマー
	//straightTimer_.Update();
	//trackTimer_.Update();

	//if (straightTimer_.IsEnd()) {
	//	trackTimer_.Start(TrackingBullet::sTrackingFrame);
	//}

	//// 追尾の速度計算処理
	//TrackUpdate();

	//// 移動
	//velocity_ += accelerate_;
	// 回転
	transform_.rotate.x += GameSystem::GameSpeedFactor() * 3.0f;
	transform_.rotate.y += GameSystem::GameSpeedFactor() * 2.0f;

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
}
