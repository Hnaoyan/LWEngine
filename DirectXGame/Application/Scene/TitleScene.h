#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Light/LightingManager.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/LevelEditor/LevelLoader.h"

#include "Application/Title/TitleObject.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public IScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// GPUの更新
	/// </summary>
	void GPUUpdate() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// UI
	/// </summary>
	void UIDraw() override;
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;
	/// <summary>
	/// モデル読み込み
	/// </summary>
	void LoadModel() override;
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture() override;

private: // システム関係
	// カメラ
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// マップデータ
	std::unique_ptr<LevelLoader> levelLoader_;
	// 箱
	std::unique_ptr<Skybox> skybox_;
	// タイトルテキスト
	Vector2 titleTextPosition_{};
	// 経過時間
	float elapsedTime_ = 0.0f;

	std::unique_ptr<LightingManager> lightManager_;

	// プレイヤーのキャラオブジェクト
	std::unique_ptr<TitleObject> playerObject_;

	bool isLoad_ = false;

	struct TransitionData
	{
		FrameTimer transitionTimer;

	};

	// テクスチャの表示用スプライト
	//std::vector<Sprite*> textures_;
	std::vector<std::pair<Vector2, Sprite*>> textures_;

	void AddSprite(const std::string& texturePath, const std::string& spriteTag);

};
