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
	void SetIsBarrage(bool isFlag) { isBarrage_ = isFlag; }

	BulletStateMachine* GetStateMachine() { return stateMachine_.get(); }
	TrackingAttribute GetTrackingType() const { return trackingType_; }
	TrackingData GetTrackingData() const { return data_; }
private:
	// 追尾の種類
	TrackingAttribute trackingType_ = TrackingAttribute::kSuperior;
	// 追跡している時間
	FrameTimer trackTimer_;
	// 直進タイマー
	FrameTimer straightTimer_;
	// 曲がる軌道用のタイマー
	FrameTimer waveTimer_;

	// 狙いがボスなら
	bool isTargetBoss_ = false;

	// 直進のフレーム数
	float straightFrame_;
	bool isBarrage_ = false;
	bool isCount = false;

	// ステートの管理
	TrackingState nowState_ = TrackingState::kStraight;
	std::optional<TrackingState> requestState_ = std::nullopt;

	// 追従のデータ構造体
	TrackingData data_{};

	// ステートの変更回数
	int32_t changeCount_ = 0;
	// ステートマシン
	std::unique_ptr<BulletStateMachine> stateMachine_;

private:
	void ChangeSelecter();
};
