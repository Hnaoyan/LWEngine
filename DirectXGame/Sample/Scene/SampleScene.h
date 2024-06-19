#pragma once
#include "../../Engine/Scene/IScene.h"
#include "../../Engine/2D/Drawer/Sprite.h"
#include "../../Engine/3D/Drawer/Model.h"
#include "../../Engine/3D/Drawer/Sphere.h"
#include "../Object/AnimSampleObject.h"
#include "../Object/AnimCubeObject.h"
#include "../../Engine/Camera/Custom/DebugCamera.h"

class SampleScene : public IScene
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

	void LoadModel() override;

	void LoadTexture() override;

	void CameraUpdate() override;
private:
	// 試し
	std::unique_ptr<AnimSampleObject> sampleObj_;
	std::unique_ptr<AnimSampleObject> walkObj_;
	std::unique_ptr<AnimCubeObject> cubeObj_;

	std::unique_ptr<Sprite> newSprite_;
	uint32_t uvTexture_ = 0u;
	Vector2 position_ = {};
	Vector4 color = { 1,1,1,1 };

	std::unique_ptr<Model> testModel_;
	std::unique_ptr<Model> walkModel_;

	std::unique_ptr<Model> cubeModel_;

	std::unique_ptr<Sphere> sphere_;

	WorldTransform testWTF_;

	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_ = false;
};
