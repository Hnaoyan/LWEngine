#pragma once
#include "../../Engine/Scene/IScene.h"
#include "../../Engine/2D/Drawer/Sprite.h"
#include "../../Engine/3D/Drawer/Model.h"
#include "../../Engine/3D/Drawer/Sphere.h"

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
private:

};
