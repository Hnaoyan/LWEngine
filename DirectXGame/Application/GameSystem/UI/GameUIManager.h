#pragma once
#include "UIStructs.h"
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

private:
	/// <summary>
	/// グローバル変数の初期化
	/// </summary>
	void InitializeGlobalValue();
};

