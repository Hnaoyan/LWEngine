#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Aim/LockOn.h"
#include "Aim/AimManager.h"

class Player;
class Input;
class BulletManager;

/// <summary>
/// 操作管理
/// </summary>
class OparationManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
private:
	/// <summary>
	/// 入力更新
	/// </summary>
	void InputUpdate();

public:
	void SetCooltime(float cooltime){
		if (!longDashCoolTimer_.IsActive()) {
			longDashCoolTimer_.Start(cooltime);
		}
	}
private: // POINTER
	// 親
	Player* player_ = nullptr;
	// 入力クラス
	Input* input_ = nullptr;

private: // USER
	bool isDash_ = false;
	int resetTime_ = 0;
	
	Vector3 dashVelocity_{};
	// 長押しダッシュ用
	FrameTimer longDashTimer_;
	FrameTimer longDashCoolTimer_;

};