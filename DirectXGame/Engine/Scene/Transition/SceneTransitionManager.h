#pragma once
#include "Engine/LwLib/LwEnginePaths.h"
#include "Engine/Utility/Singleton.h"

class Sprite;

class SceneTransitionManager : public Singleton<SceneTransitionManager>
{
public:
	SceneTransitionManager();
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
public:
	// 色
	static Vector4 sColor;

private:
	// 画面用のテクスチャ
	Sprite* transitionSprite_ = nullptr;

	// 折り返し地点までの時間
	FrameTimer setupTimer_;
	// 戻ってくる時間
	FrameTimer returnTimer_;
};
