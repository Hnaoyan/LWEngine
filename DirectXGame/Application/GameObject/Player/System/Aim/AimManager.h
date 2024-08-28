#pragma once
#include "Engine/3D/WorldTransform.h"
#include "Engine/Input/Input.h"
#include "Engine/2D/Drawer/Sprite.h"

class Player;
class ICamera;

class AimManager
{
private:
	struct ResetValue {
		bool isReceivingInput;
		float rate;
	};

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
	/// レティクルの描画
	/// </summary>
	void Draw();

private: // ユーザー

public: // アクセッサ
	Vector3 GetWorldPosition() { return targetPosition_; }

private: // 
	// プレイヤー
	Player* player_ = nullptr;
	// 入力
	Input* input_ = nullptr;

private: // User
	// プレイヤーからのオフセット
	WorldTransform offSetTransform_;
	Matrix4x4 parentMatrix_{};
	// オフセットの位置
	Vector3 offsetPosition_;
	// エイムの位置
	Vector3 targetPosition_{};
	// エイムのスクリーン上座標
	Vector2 screenPosition_ = {};
	// スプライト
	std::unique_ptr<Sprite> reticleSprite_;
	// 
	ResetValue reset_{};
};
