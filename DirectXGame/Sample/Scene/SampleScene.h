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
private:
	std::unique_ptr<Particle> particles_;
	struct planeInfo {
		WorldTransform worldTransform;
		Model* model;
	};
	planeInfo plane_;
private:
	std::unique_ptr<LevelLoader> levelLoader_;

	// 試し
	std::array<std::unique_ptr<AnimCubeObject>, 2> cubes_;
	std::array<std::unique_ptr<AnimSampleObject>, 3> humans_;

	std::unique_ptr<GPUParticleSystem> gpuParticle_;

	std::unique_ptr<PlSampleObject> player_;

	std::list<std::unique_ptr<InstancedGroup>> group_;
	std::unique_ptr<InstancedGroup> testGroup1_;

	struct spriteInfo {
		uint32_t uvTexture_ = 0u;
		Vector2 position_ = {};
		Vector4 color = { 1,1,1,1 };
		EulerTransform spriteTransform_{};
		bool isInvisible_ = false;
	};
	std::unique_ptr<Sprite> newSprite_;
	spriteInfo newSpriteData_;

	std::unique_ptr<Model> testModel_;
	std::unique_ptr<Model> walkModel_;
	std::unique_ptr<Model> sneakWalkModel_;

	Model* cubeModel_;

	Vector3 generatePosition_ = {};

	std::unique_ptr<Skydome> sphere_;

	WorldTransform testWTF_;

	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_ = false;

	std::unique_ptr<Skybox> skybox_;

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
