#pragma once
#include "Engine/Math/MathLib.h"
#include <string>

class Sprite;

namespace GameUI
{
	struct SpriteInfo {
		Sprite* sprite;	// スプライト
		Vector4 color;	// 色
		// 初期化
		void Initialize(std::string spriteTag);
		// 更新
		void Update();
	};

	struct BarRenderInfo {
		// 背景
		SpriteInfo backGround;
		// 現在体力
		SpriteInfo currentHP;
		// 減少体力
		SpriteInfo decreaseHP;
		// 座標
		Vector2 position;
		// 最大スケール
		Vector2 maxScale;
		// 現在のスケール
		Vector2 currentScale;
	};

	class UIManager
	{
	public:
		void Initialize();
		void Draw();

	};

}
