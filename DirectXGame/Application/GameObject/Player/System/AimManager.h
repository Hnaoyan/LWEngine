#pragma once
#include "../../../../Engine/3D/WorldTransform.h"
#include "../../../../Engine/Input/Input.h"
#include "../../../../Engine/2D/Drawer/Sprite.h"

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

	void Draw();

private: // ユーザー
	/// <summary>
	/// 右スティックによるエイムの移動処理
	/// </summary>
	void AimUpdate();

public: // アクセッサ
	Vector3 GetWorldPosition() { return offSetTransform_.GetWorldPosition(); }

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

	std::unique_ptr<Sprite> reticleSprite_;

	// 
	ResetValue reset_{};

};
