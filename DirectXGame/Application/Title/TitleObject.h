#pragma once
#include "../GameObject/IGameObject.h"

class TitleObject : public IGameObject
{
private:
	enum class AnimationType
	{
		Normally,	// 通常の状態
		Starting,	// 開始時への状態

	};

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

	// 恒常処理
	void NormallyInitialize();
	void NormallyUpdate();

	// 開始処理
	void StartingInitialize();
	void StartingUpdate();


private:
	std::unique_ptr<Material> material_;
	// 回転中か
	bool isRotate_ = true;

	// アニメーションの状態
	AnimationType e_AnimState_;	
	// アニメーションのタイマー
	FrameTimer animTimer_;
	float startEulerY_ = 0.0f;
};
