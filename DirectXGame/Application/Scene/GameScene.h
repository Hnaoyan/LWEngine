#pragma once
#include "../../Engine/Scene/IScene.h"
#include "../../Engine/2D/Drawer/Sprite.h"
#include "../../Engine/3D/Drawer/3DDrawers.h"
#include "../../Engine/Camera/CameraList.h"
#include "../GameObject/GameObjectLists.h"

class GameScene : public IScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

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

	/// <summary>
	/// カメラ更新
	/// </summary>
	void CameraUpdate() override;

	/// <summary>
	/// ライト初期化
	/// </summary>
	void LightingInitialize() override;

private: // アプリ
	std::unique_ptr<Player> player_;
	std::vector<std::unique_ptr<Model>> models_;

private: // システム関係
	// カメラ君
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
	bool isDebugCamera_ = false;

	// ライト君
	std::unique_ptr<DirectionalLight> directionalLight_;
	CBufferDataDirectionalLight lightData_;
	std::unique_ptr<SpotLight> spotLight_;
	CBufferDataSpotLight spLightData_;
	std::unique_ptr<PointLight> pointLight_;
	CBufferDataPointLight ptLightData_;
};
