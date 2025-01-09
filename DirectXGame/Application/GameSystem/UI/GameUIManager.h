#pragma once
#include "UIStructs.h"
#include "../GameSystemStructs.h"
#include "Engine/LwLib/LwEnginePaths.h"
#include <vector>

class GameObjectManager;
class GameSystem;

/// <summary>
/// オブジェクトに依存しないUI・UIのグローバル変数管理クラス
/// </summary>
class GameUIManager
{
public:
	GameUIManager(GameSystem* gameSystem);
	void Initialize();
	void Update(GameSceneState state);
	void Draw(GameObjectManager* gameObjectManager);
public:	// アクセッサ
	void SetIsHudHide(bool isHide) { isHUDHide_ = isHide; }
	bool IsHUDHide() { return isHUDHide_; }
private:// SYSTEM
	// HUDの追加
	void AddHUD(std::string name, uint32_t texture);
	// ゲームシステム
	GameSystem* gameSystem_ = nullptr;
	// HUDを消しておくかのフラグ
	bool isHUDHide_ = false;

	GameSceneState sceneState_ = GameSceneState::kWait;

private: // UI
	// HUD
	std::vector<GameUI::UIData> hudElements_;
	// クリアテキスト
	GameUI::TextUI gameClear_ = {};
	// オーバーテキスト
	GameUI::TextUI gameOver_ = {};
	// タイトルに戻るテキスト
	GameUI::TextUI titleBack_ = {};
	// リプレイテキスト
	GameUI::TextUI replay_ = {};
	// カメラ変更テキスト
	GameUI::TextUI cameraChange_ = {};

	// ゲームの開始テキスト
	struct TextComp
	{
		FrameTimer timer;
		float returnFrame;
		void Initialize() {
			backColor = Vector4(0.0f, 0.0f, 0.0f, 0.95f);
			Start();
		}
		void Start() {
			timer.Start(returnFrame);
		}
		void Update() {
			// タイマー更新
			timer.Update();
			if (timer.IsEnd()) {
				if (isAlpha) {
					isAlpha = false;
				}
				else {
					isAlpha = true;
				}
				Start();
			}
			float min = 0.6f;
			float max = 1.0f;
			if (isAlpha) {
				text.color.w = Ease::Easing(min, max, timer.GetElapsedFrame());
			}
			else {
				text.color.w = Ease::Easing(max, min, timer.GetElapsedFrame());
			}
			// スプライトへの適応
			text.sprite->SetColor(text.color);
			text.sprite->SetSize(text.size);
			text.sprite->SetPosition(text.position);
			// 背景
			backSprite->SetColor(backColor);
			backSprite->SetPosition(text.position);
		}
		bool isAlpha;	// true:0→1 false:1→0
		GameUI::TextUI text;
		Sprite* backSprite = nullptr;
		Vector4 backColor = {};
		Vector2 defaultSize = {};
	};

	TextComp gameStart_ = {};

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

