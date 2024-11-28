#pragma once
#include "Engine/LwLib/Utillity/FrameTimer.h"
#include "Aim/LockOn.h"
#include "Aim/AimManager.h"

class Player;
class Input;
class BulletManager;

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
	void SetEnemyList(std::vector<std::unique_ptr<SampleEnemy>>* lists) { lockOn_.SetEnemyList(lists); }
	PlayerContext::AimManager* GetAimManager() { return &aimManager_; }
	PlayerContext::LockOn* GetLockOn() { return &lockOn_; }
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
	
	FrameTimer shotTimer_;
	FrameTimer lockOnCooltime_;

	Vector3 dashVelocity_{};
	// 長押しダッシュ用
	FrameTimer longDashTimer_;
	FrameTimer longDashCoolTimer_;

private: // SYSTEM
	// ロックオンシステム
	PlayerContext::LockOn lockOn_;
	// エイム関係
	PlayerContext::AimManager aimManager_;

};