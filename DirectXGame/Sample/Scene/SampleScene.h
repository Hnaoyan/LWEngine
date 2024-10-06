#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "../Object/AnimSampleObject.h"
#include "../Object/AnimCubeObject.h"
#include "../Object/PlSampleObject.h"
#include "Engine/Camera/Custom/DebugCamera.h"
#include "Engine/Light/LightLists.h"
#include "Engine/PostEffect/PostEffectData.h"
#include "Engine/3D/Instancing/InstancedGroup.h"
#include "Engine/LevelEditor/LevelLoader.h"
#include "Engine/Particle/Particle.h"
#include "Engine/Particle/GPUParticleSystem.h"
#include "Engine/3D/User/MissileTrail.h"

#include <list>

class SampleScene : public IScene
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
	/// UI描画
	/// </summary>
	void UIDraw() override;

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw() override;

	void LoadModel() override;

	void LoadTexture() override;

	void CameraUpdate() override;

	void LightingInitialize() override;

	void PostChanger();

private:
	std::unique_ptr<Particle> particles_;
	struct planeInfo {
		WorldTransform worldTransform;
		Model* model;
	};
	planeInfo plane_;
private: // USER
	// 軌跡クラス
	std::unique_ptr<MissileTrail> trail_;
	// 線描画
	std::unique_ptr<Line3D> lines_;
	// 三角形
	std::unique_ptr<Triangle3D> triangle_;

	std::unique_ptr<LevelLoader> levelLoader_;

	std::unique_ptr<GPUParticleSystem> gpuParticle_;

	Vector3 generatePosition_ = {};

	std::unique_ptr<Skydome> sphere_;
	std::unique_ptr<Material> cubeMaterial_;

	WorldTransform skyboxTransform_;
	std::unique_ptr<Skybox> skybox_;

	// カーブ用の座標
	std::vector<Vector3> curvePoints_;

private: // SYSTEM
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_ = false;
	// ライト君
	std::unique_ptr<DirectionalLight> directionalLight_;
	CBufferDataDirectionalLight lightData_;

	std::unique_ptr<SpotLight> spotLight_;
	CBufferDataSpotLight spLightData_;

	std::unique_ptr<PointLight> pointLight_;
	CBufferDataPointLight ptLightData_;

	CBufferDataBlur blurData_;
	CBufferDataVignette vignetteData_;
	CBufferDataDissolve dissolveData_;
	CBufferDataNoise noiseData_;
	CBufferDataHSV hsvData_;
	CBufferDataBloom bloomData_;

	int postEffecter_ = 0;

};
