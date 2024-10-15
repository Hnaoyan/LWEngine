#pragma once
#include "../IBullet.h"

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
	// 追尾の種類
	enum class TrackingType : uint32_t
	{
		kStandard,	// 通常
		kInferior,	// 劣等
		kSuperior,	// 優等
	};
	void SetGameObject(IGameObject* object) { object_ = object; }
	void SetTrackType(TrackingType type) { trackingType_ = type; }

private:
	IGameObject* object_ = nullptr;
	TrackingType trackingType_ = TrackingType::kStandard;
};
