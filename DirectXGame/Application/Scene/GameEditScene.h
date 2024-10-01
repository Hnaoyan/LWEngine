#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/2D/SpriteManager.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "../GameObject/GameObjectLists.h"
#include "Application/GameSystem/GameSystem.h"
#include "Application/GameSystem/UI/GameUIManager.h"

class GameEditScene :public IScene
{
public:
	// 初期化
	void Initialize() override;
	// GPU
	void GPUUpdate() override;
	// CPU
	void Update() override;
	// 描画
	void Draw() override;
	// UI
	void UIDraw() override;
	// ImGui
	void ImGuiDraw() override;
	// モデル読み込み
	void LoadModel() override;
	// テクスチャ読み込み
	void LoadTexture() override;
	// カメラ
	void CameraUpdate() override;
	// ライト初期化
	void LightingInitialize() override;
	// ライトの更新
	void LightingUpdate();

private:
	std::unique_ptr<Boss> boss_;
	// GPUParticle
	std::unique_ptr<GPUParticleSystem> gpuParticleManager_;

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
