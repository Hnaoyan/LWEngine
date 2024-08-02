#pragma once
#include "../../Engine/Scene/IScene.h"
#include "../../Engine/2D/Drawer/Sprite.h"
#include "../../Engine/3D/Drawer/3DDrawers.h"
#include "../../Engine/3D/ModelManager.h"
#include "../../Engine/Camera/CameraList.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/LevelEditor/LevelLoader.h"

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

	void AddSprite(Vector2 position,uint32_t texture);

private: // システム関係
	// カメラ君
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	bool isDebugCamera_ = false;
	// マップデータ
	std::unique_ptr<LevelLoader> levelLoader_;
	// 箱
	std::unique_ptr<Skybox> skybox_;

	std::vector<std::unique_ptr<Sprite>> uiSprites_;
	Vector2 size_ = {};

	// ライト君
	std::unique_ptr<DirectionalLight> directionalLight_;
	CBufferDataDirectionalLight lightData_;
	std::unique_ptr<SpotLight> spotLight_;
	CBufferDataSpotLight spLightData_;
	std::unique_ptr<PointLight> pointLight_;
	CBufferDataPointLight ptLightData_;
};
