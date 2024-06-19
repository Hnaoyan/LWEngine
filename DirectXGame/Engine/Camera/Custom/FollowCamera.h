#pragma once
#include "../Core/ICamera.h"
#include "../../3D/WorldTransform.h"

class FollowCamera : public ICamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

public:
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
	/// <summary>
	/// オフセット作成
	/// </summary>
	/// <returns></returns>
	Vector3 CreateOffset() const;

public:
	/// <summary>
	/// 対象の設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* target) {
		target_ = target;
		Reset();
	}

private:
	// ターゲット
	const WorldTransform* target_ = nullptr;
	// デフォルトオフセット
	Vector3 defaultOffset_ = {};
	// 残像座標
	Vector3 interTarget_ = {};
	// 目標地点の角度
	float destinationAngleY_ = 0.0f;
	float delayRate_ = 0.01f;
};
