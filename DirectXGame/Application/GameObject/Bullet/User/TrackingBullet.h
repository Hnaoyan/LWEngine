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

private:
	// 追尾の種類
	TrackingType trackingType_ = TrackingType::kStandard;
	// 追跡している時間
	FrameTimer trackTimer_;
	// 直進タイマー
	FrameTimer straightTimer_;
	// 補間の半径
	float lerpRadius_ = 0.0f;
	// 劣等型の狙い先のオフセット位置
	Vector3 offset_;

	// 狙いがボスなら
	bool isTargetBoss_ = false;

	// 曲がる軌道用のタイマー
	FrameTimer waveTimer_;

	// ステートの管理
	TrackingState nowState_;
	std::optional<TrackingState> requestState_ = std::nullopt;

	float waveCount_ = 0.0f;

private:
	void SetupByType();
	void TrackUpdate();
	void WaveUpdate();
private:
	// 通常
	void CalcStandardMissile();
	// 劣等
	void CalcInferiorMissile();
	// 優等
	void CalcSuperiorMissile();

public: // 共通
	// 対尾の時間
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
