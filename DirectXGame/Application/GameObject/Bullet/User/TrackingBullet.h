#pragma once
#include "../IBullet.h"
#include "../BulletEnums.h"

#include <optional>

/// <summary>
/// 追従の弾
/// </summary>
class TrackingBullet : public IBullet
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;
	/// <summary>
	/// コールバック関数
	/// </summary>
	/// <param name="object"></param>
	void OnCollision(ColliderObject object) override;

public: // アクセッサ
	void SetTrackType(TrackingAttribute type) { trackingType_ = type; }
	void SetIsBarrage(bool isFlag) { isFlag; }
	void SetStraightFrame(const float& frame) { straightFrame_ = frame; }
	
	BulletStateMachine* GetStateMachine() { return stateMachine_.get(); }
	TrackingAttribute GetTrackingType() const { return trackingType_; }
	TrackingData GetTrackingData() const { return data_; }
	// true:ボス false:プレイヤー
	bool IsTargetBoss() const { return isTargetBoss_; }
private:
	// 追尾の種類
	TrackingAttribute trackingType_ = TrackingAttribute::kSuperior;
	// 直進のフレーム数
	float straightFrame_;
	// 狙いがボスなら
	bool isTargetBoss_ = false;
	// 追従のデータ構造体
	TrackingData data_{};
	// 初期の向き
	Vector3 defaultTargetDirection_ = {};

	// ステートマシン
	std::unique_ptr<BulletStateMachine> stateMachine_;
	// 変更を管理するクラス
	std::unique_ptr<StateTransitionManager> transitionManager_;
};
