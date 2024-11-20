#pragma once
#include "Engine/Math/MathLib.h"
#include <string>
#include <vector>
#include <memory>

class Sprite;

namespace GameUI
{
	// UI用のデータ
	struct UIData {
		std::unique_ptr<Sprite> sprite;	// スプライト
		std::string tag;	// 名前
		uint32_t num = 0;	// 番号
		Vector2 position{};	// 座標
		Vector2 scale{};	// スケール
		uint32_t texture = 0;	// テクスチャ
	};

	// スプライトのデータ
	struct SpriteInfo {
		Sprite* sprite;	// スプライト
		Vector4 color;	// 色
		// 初期化
		void Initialize(std::string spriteTag);
		// 更新
		void Update();
		// 描画
		void Draw();
	};

	// HPバー用の構造体
	struct BarRenderInfo {
		SpriteInfo backGround;	// 背景
		SpriteInfo currentHP;	// 現在体力
		SpriteInfo decreaseHP;	// 減少体力

		Vector2 position;	// 座標
		Vector2 maxScale;	// 最大スケール
		Vector2 currentScale;	// 現在のスケール
	};
}