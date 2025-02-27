#pragma once
#include "Engine/3D/Instancing/InstancedUnit.h"
#include "Engine/LwLib/LwEnginePaths.h"

class BulletBombEffect : public InstancedUnit
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	void SetGoalScale(const Vector3& scale) { goalScale_ = scale; }

private:
	// 初期スケール
	Vector3 defaultScale_ = {};
	Vector3 goalScale_ = {};
	// 速度ベクトル
	Vector3 velocity_ = {};
	// 削除時間
	FrameTimer deleteTimer_;

};