#pragma once

#include "../IGameObject.h"
#include "../../../Engine/LwLib/Utillity/FrameTimer.h"

class SampleBullet : public IGameObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="desc"></param>
	void Draw(ModelDrawDesc desc) override;
	/// <summary>
	/// ImGui描画
	/// </summary>
	/// <param name="name"></param>
	void ImGuiDraw() override;

public:
	void SetPosition(const Vector3& position) {
		worldTransform_.transform_.translate = position;
		worldTransform_.UpdateMatrix();
	}

	Vector3 velocity_ = {};
	// 死亡フラグ
	bool isDead_ = false;

	FrameTimer deathTimer_;
};
