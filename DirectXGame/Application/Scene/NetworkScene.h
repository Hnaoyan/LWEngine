#pragma once
#include "Engine/Scene/IScene.h"
#include "Engine/2D/Drawer/Sprite.h"
#include "Engine/3D/Drawer/3DDrawers.h"
#include "Engine/3D/ModelUtility/ModelManager.h"
#include "Engine/Camera/CameraList.h"
#include "Engine/Light/LightingManager.h"
#include "Engine/Collision/CollisionManager.h"
#include "Engine/LevelEditor/LevelLoader.h"

class NetworkScene : public IScene
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

};
