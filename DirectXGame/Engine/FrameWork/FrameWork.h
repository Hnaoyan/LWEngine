#pragma once
#include "../WindowAPI/WindowAPI.h"
#include "../Base/DirectXCommon.h"
#include "../Input/Input.h"

#include "../Base/ImGui/ImGuiManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Transition/SceneTransitionManager.h"
#include "../Audio/AudioManager.h"
#include "../3D/ModelUtility/ModelManager.h"
#include "../2D/TextureManager.h"
#include "../2D/SpriteManager.h"

/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:
	/// <summary>
	/// 全体の処理をまとめる関数
	/// </summary>
	void Execute();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ImGui
	/// </summary>
	virtual void ImGuiDraw() = 0;

	/// <summary>
	/// 終了チェック
	/// </summary>
	/// <returns></returns>
	virtual bool IsEndRequest() { return endRequest_; }

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Framework() = default;
protected:
	// 終了フラグ
	bool endRequest_;

protected:	// ポインタ
	// ウィンドウAPI
	WindowAPI* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;

protected:
	// マネージャ関係
	std::unique_ptr<SceneManager> sceneManager_;	// シーンマネージャ
	std::unique_ptr<SceneTransitionManager> transitionManager_;	// シーン遷移用マネージャー
	std::unique_ptr<ImGuiManager> imGuiManager_;	// ImGuiマネージャ
	std::unique_ptr<ModelManager> modelManager_;	// モデルマネージャ
	std::unique_ptr<SpriteManager> spriteManager_;	// スプライトマネージャ
	std::unique_ptr<TextureManager> textureManager_;	// テクスチャマネージャ
	std::unique_ptr<AudioManager> audioManager_;	// 音源マネージャ
};

