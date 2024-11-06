#pragma once
#include "../IBullet.h"
#include "../BulletEnums.h"

#include <optional>

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
	void SetTrackType(TrackingType type) { trackingType_ = type; }
	TrackingType GetTrackingType() { return trackingType_; }
private:
	// 追尾の種類
	TrackingType trackingType_ = TrackingType::kSuperior;
	// 追跡している時間
	FrameTimer trackTimer_;
	// 直進タイマー
	FrameTimer straightTimer_;
	// 曲がる軌道用のタイマー
	FrameTimer waveTimer_;

	// 狙いがボスなら
	bool isTargetBoss_ = false;

	// ステートの管理
	TrackingState nowState_ = TrackingState::kStraight;
	std::optional<TrackingState> requestState_ = std::nullopt;

	TrackingData data{};

private:
	void ChangeSelecter();

public: // 共通
	// 追従の時間
	static float sTrackingFrame;
	// 減衰値
	static float sDamping;
	// 広がり
	static float sLerpRadius;
	// 基本の速度
	static float sBaseVelocity;
	// 初期速度
	static float sInitSpeed;
};
