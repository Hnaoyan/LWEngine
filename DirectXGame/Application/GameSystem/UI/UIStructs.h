#pragma once
#include "Engine/Math/MathLib.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/3D/Drawer/Model.h"
#include <string>
#include <vector>
#include <memory>

namespace GameUI
{
	// UI用のデータ
	struct UIData {
		Sprite* sprite;	// スプライト
		std::string tag;	// 名前
		Vector2 position{};	// 座標
		Vector2 scale{};	// スケール
		uint32_t texture = 0;	// テクスチャ

		void ImGuiDraw();

	};

	// クリア・オーバー等の別途テキストUI用
	struct TextUI
	{
		Sprite* sprite = nullptr;
		Vector2 position = {};
		Vector2 size = {};
		Vector4 color = {};
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

	// 
	struct BillBoardRenderInfo {
		Model* model;
		std::unique_ptr<Material> material;
		WorldTransform worldTransform;
		Vector3 maxScale{};
		uint32_t texture;
		Vector3 offset{};
		bool isInvisible_ = false;
		void Update() {
			worldTransform.transform_.translate += offset;
		}
	};
}
