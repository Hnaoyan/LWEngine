#pragma once
#include "../../../../Engine/3D/WorldTransform.h"
#include "../../../../Engine/Input/Input.h"

class Player;
class ICamera;

class AimManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player"></param>
	void Initialize(Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(ICamera * camera);
	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// 右スティックによるエイムの移動処理
	/// </summary>
	void AimUpdate();

private: // 
	// プレイヤー
	Player* player_ = nullptr;
	// 入力
	Input* input_ = nullptr;

private: // User
	// エイムの位置
	WorldTransform offSetTransform_;
	// エイムのスクリーン上座標
	Vector2 screenPosition_ = {};
};
